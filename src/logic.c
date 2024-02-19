#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

#include "utils.h"
#include "logic.h"

// Initialise les trains et leur position
Train init_train(int id, Track *track_list, int track_list_id, int start) {
  int x = track_list[start].x;
  int y =  track_list[start].y;
  Train train = { x, y, 0.0, MAX_SPEED, id, track_list_id, start };
  return train;
}

// Réponse des trains à une consigne de vitesse
float next_speed(float speed, float u_speed) {
  if (fabs(u_speed - speed) > 0.0001) {
    float delta_v = (TIME_STEP/RESPONSE_TIME) * (u_speed - speed);
    return speed + delta_v;
  }
  else if (fabs(u_speed) < 0.0001) {
    return 0.0;
  } 
  return speed;
}

float stopping_distance(float initial_speed) {
  float speed = initial_speed;
  float distance = 0.0f;

  while (fabs(speed) > 0.0001) {
    speed = next_speed(speed, 0.0f);
    distance += speed * TIME_STEP;
  }
  return distance;
}

// Distance entre deux tracks
float distance_tracks(Track track_a, Track track_b) {
  float dx = (float)(track_a.x - track_b.x);
  float dy = (float)(track_a.y - track_b.y);
  return sqrtf(dx * dx + dy * dy);
}

// Distance d'un train à un rail
float distance_to_track(Train train, Track track) {
  float dx = (float)track.x - train.x;
  float dy = (float)track.y - train.y;
  return sqrtf(dx * dx + dy * dy);
}

// Distance du train a au train b; -1 si elle n'existe pas
float distance_trains(Train train_a, Train train_b, Track *track_list_a, int track_len_a, Track track_b) {
  int current_track_id = train_a.last_track;
  float distance = 0.0;

  if (!(in_track(track_list_a, track_len_a, track_b))) {
    return -1.0;
  }

  int track_b_index = track_index(track_list_a, track_len_a, track_b); 

  for (int i = 0; i < track_len_a; i++) {
    int index = (current_track_id + i) % track_len_a;
    Track current_track = track_list_a[index];

    if (index == track_b_index) {
      float dx = (float)(train_a.x - train_b.x);
      float dy = (float)(train_a.y - train_b.y);
      return distance + sqrtf(dx * dx + dy * dy);
    }

    if (i < track_len_a - 1) {
      Track next_track = track_list_a[(index + 1) % track_len_a];
      distance += distance_tracks(current_track, next_track);
    }
  }
  return -1.0;
}

// Calcul de la nouvelle position d'un train compte tenu de son parcours et de sa vitesse
// Produit un effet de bord
void calculate_next_position(Train *train, Track *track_list, int track_len) {
  int track_id = train->last_track;
  float speed = train->speed;

  float x = train->x;
  float y = train->y;

  float d_track = distance_to_track((*train), track_list[(track_id + 1) % track_len]);
  
  while (d_track < speed) {
    speed -= d_track;
    track_id = (track_id + 1) % track_len;
    
    x = (float)track_list[track_id].x;
    y = (float)track_list[track_id].y;

    d_track = distance_tracks(track_list[track_id], track_list[track_id + 1]);
  }

  int next_track_id = (track_id + 1) % track_len;
  Track currrent_track = track_list[track_id];
  Track next_track = track_list[next_track_id];

  float x_next = (float)next_track.x;
  float y_next = (float)next_track.y;

  float dx = x_next - x;
  float dy = y_next - y;

  float unit_x = dx / d_track;
  float unit_y = dy/ d_track;
  train->x += unit_x * speed;
  train->y += unit_y * speed;
  
  train->last_track = track_id;
  train->speed = next_speed(train->speed, train->u_speed);
}

// Simulates the position of train after a certain distance
float* simulate_movement(Train *train, Track *track_list, int track_len, float distance) {
  int track_id = train->last_track;
  float speed = distance;

  float *new_coord = (float *)malloc(2 * sizeof(float));
  if (new_coord == NULL) {
    printf("Malloc failled\n");
    exit(EXIT_FAILURE);
  }

  new_coord[0] = train->x;
  new_coord[1] = train->y;

  float d_track = distance_to_track((*train), track_list[(track_id + 1) % track_len]);
  
  while (d_track < speed) {
    speed -= d_track;
    track_id = (track_id + 1) % track_len;
    
    new_coord[0] = (float)track_list[track_id].x;
    new_coord[1] = (float)track_list[track_id].y;

    d_track = distance_tracks(track_list[track_id], track_list[(track_id + 1) % track_len]);
  }

  int next_track_id = (track_id + 1) % track_len;
  Track next_track = track_list[next_track_id];

  float dx = next_track.x - new_coord[0];
  float dy = next_track.y - new_coord[1];

  float unit_x = dx / d_track;
  float unit_y = dy/ d_track;

  new_coord[0] += unit_x * speed;
  new_coord[1] += unit_y * speed;
  
  return new_coord;
}

// Détecte si 2 trains sont en collision
bool detect_collision(Train *train_a, Train *train_b, float radius) {
  float dx = fabs(train_a->x - train_b->x);
  float dy = fabs(train_a->y - train_b->y);
  if ((dx < radius) && (dy < radius)) {
    return true;
  }
  return false;  
}

// Liste toutes les track partagés
Track* critical_sections(Track **tracks_list, int *tracks_len, int tracks_nb, int *size_out) {
  int max_len = 0;
  for (int i = 0; i < tracks_nb; i++) {
    int len = tracks_len[i];
    if (len > max_len) max_len = len;
  }

  Track *critical = malloc(sizeof(Track) * max_len);
  if (!critical) {
    *size_out = -1;
    return NULL;
  }
  int counter = 0;
  
  for (int i = 0; i < tracks_nb - 1; i++) {
    for (int j = 0; j < tracks_len[i]; j++) {
      Track track = tracks_list[i][j];
      
      for (int k = i + 1; k < tracks_nb; k++) {
        if (in_track(tracks_list[k], tracks_len[k], track)) {
          if (!(in_track(critical, counter, track))) {
            Track common_track = { track.x, track.y, track.section, true, -1 };
            critical[counter++] = common_track;
          }
        }
      }
    }
  }

  Track *shrinked_critical = malloc(sizeof(Track) * counter);
  if (!shrinked_critical) {
    *size_out = -1;
    return NULL;
  }
  for (int i = 0; i < counter; i++) {
    shrinked_critical[i] = critical[i];
  }

  free(critical);

  *size_out = counter;
  return shrinked_critical;
}

// Section à double sens
Track* critical_sections_man(int *size_out) {
    Track *critical = malloc(sizeof(Track) * 12);
    if (!critical) {
        *size_out = -1;
        return NULL;
    }
  
    for (int i = 0; i < 9; i++) {
        Track critical_section = {6 + i, 12, 0, true, -1};
        critical[i] = critical_section;
    }
    
    Track critical_section_10 = {15, 11, 0, true, -1};
    critical[10] = critical_section_10;
    
    Track critical_section_11 = {15, 10, 0, true, -1};
    critical[11] = critical_section_11;

    *size_out = 12;
    return critical;
}

int nearest_track(Track *track_list, int track_len, float x, float y) {
  int nearestIndex = -1;
  float nearestDistance = FLT_MAX;
  float distance;

  for (int i = 0; i < track_len; i++) {
    Track current_track = track_list[i];
    distance = sqrtf((current_track.x - x) * (current_track.x - x) + (current_track.y - y) * (current_track.y - y));

    if (distance < nearestDistance) {
      nearestDistance = distance;
      nearestIndex = i;
    }
  }

  return nearestIndex;
}

// Commande séquentielle des trains
float new_speed(Track **tracks_list, int *tracks_len, Train **trains, int trains_nb,
                int train_id, Track *critical, int critical_len, Track *shared, int shared_len) {
  Train* train = trains[train_id];
  Track* track_list = tracks_list[train->track_list];
  int track_len = tracks_len[train->track_list];
  int last_track = train->last_track;
  Track current_track = track_list[last_track];

  float stop_distance = stopping_distance(train->speed);
  float *next_coord = simulate_movement(train, track_list, track_len, stop_distance);
  float *nnext_coord = simulate_movement(train, track_list, track_len, 2 * stop_distance); 

  int nt_id = nearest_track(track_list, track_len, next_coord[0], next_coord[1]);
  int nnt_id = nearest_track(track_list, track_len, nnext_coord[0], nnext_coord[1]);

  Track next_track = track_list[nt_id];
  Track nnext_track = track_list[nnt_id];

  // Initialisaiton de la vitesse max en fonction de si l'on est dans un virage
  float u_max = MAX_SPEED;
  if ((current_track.x != next_track.x) && (current_track.y != next_track.y)) {
    u_max = MAX_SPEED_TURN;
  }
  if ((next_track.x != nnext_track.x) && (next_track.y != nnext_track.y)) {
    u_max = MAX_SPEED_TURN;
  }

  // Si l'on sort d'une section critique, on libère
  if (!(in_track(critical, critical_len, track_list[last_track]))) {
    for (int i = idx(last_track - 1, track_len);
         in_track(critical, critical_len, track_list[idx(i, track_len)]);
         i--) {
      int crit_index = track_index(critical, critical_len, track_list[idx(i, track_len)]);
      if (critical[crit_index].r_id == train_id) {
        critical[crit_index].available = true;
        critical[crit_index].r_id = -1;
        DEBUG_PRINT("R-Train %d unlocking critical section %d %d\n", train_id, critical[crit_index].x, critical[crit_index].y);
      }
    }
  }

  // Si l'on sort d'une section partagée, on libère
  if (in_track(shared, shared_len, track_list[idx(last_track - 1, track_len)])) {
    int shrd_index = track_index(shared, shared_len,
                                 track_list[idx(last_track - 1, track_len)]);
    if (shared[shrd_index].r_id == train_id) {
      shared[shrd_index].available = true;
      shared[shrd_index].r_id = -1;
      DEBUG_PRINT("Train %d unlocking shared section %d %d\n",
                  train_id, shared[shrd_index].x, shared[shrd_index].y);
    }
  }

  // Si l'on va rentrer dans une section critique
  if (in_track(critical, critical_len, next_track)) {
    int crit_index = track_index(critical, critical_len, next_track);

    // Réservation de la prochaine section critique
    if (critical[crit_index].available) {
      critical[crit_index].available = false;
      critical[crit_index].r_id = train_id;
      DEBUG_PRINT("Train %d locking critical section %d %d\n", train_id, critical[crit_index].x, critical[crit_index].y);
      for (int i = nt_id + 1; in_track(critical, critical_len, track_list[i % track_len]); i++) {
        int crit_index = track_index(critical, critical_len, track_list[i % track_len]);
        if (critical[crit_index].available) {
          critical[crit_index].available = false;
          critical[crit_index].r_id = train_id;
          DEBUG_PRINT("R-Train %d locking critical section %d %d\n", train_id, critical[crit_index].x, critical[crit_index].y);
        } else {
          DEBUG_PRINT("\tLock: This shouldn't happen...\n");
        }
      }
      return u_max;
    }
    // Si le train a réservé la section
    else if (critical[crit_index].r_id == train_id) {
      return u_max;
    }
    // Sinon on doit laisser passer
    else {
      return 0.0;
    }
  }

  // Si l'on va emprunter une section partagée
  if (in_track(shared, shared_len, nnext_track)) {
    int shrd_index = track_index(shared, shared_len, nnext_track);

    // Réservation de la prochaine section partagée
    if (shared[shrd_index].available) {
      shared[shrd_index].available = false;
      shared[shrd_index].r_id = train_id;
      DEBUG_PRINT("Train %d locking shared section %d %d\n", train_id, shared[shrd_index].x, shared[shrd_index].y);
      return u_max;
    }
    // Si le train a réservé la section
    else if (shared[shrd_index].r_id == train_id) {
      return u_max;
    }
    // Sinon on doit laisser passer
    else {
      return 0.0;
    }
  }
  
  // Gestion de la distance inter trains
  for (int i = last_track; i < trains_nb; i++) {
    if (i != train_id) {
      float d = distance_trains(*train, *trains[i], track_list, track_len,
                                tracks_list[trains[i]->track_list][trains[i]->last_track]);
      if ((d > 0) && (d < 2 * stop_distance)) {
        if (d < stop_distance) {
          DEBUG_PRINT("Train %d to close to train %d, slowing down\n", train_id, i);
          return 0.5 * u_max;
        } else {
          DEBUG_PRINT("Train %d to close to train %d, stopping\n", train_id, i);
          return 0.0;
        }
      }
    }
  }

  return u_max;
}

