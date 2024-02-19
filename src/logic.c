#include <stdlib.h>
#include <math.h>
#include <stdio.h>

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
    if (fabs(u_speed - speed) > 0.00001) {
      float delta_v = (TIME_STEP/RESPONSE_TIME) * (u_speed - speed);
      return speed + delta_v;
    }
    return speed;
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

  for (int i = 0; i < track_len_a; i++) {
    current_track_id = (current_track_id + i) % track_len_a;
    Track current_track = track_list_a[current_track_id];
    Track next_track = track_list_a[(current_track_id + 1) % track_len_a];

    if (same_track(current_track, track_b)) {
      float dx = (float)(train_a.x - train_b.x);
      float dy = (float)(train_a.y - train_b.y);
      return distance + sqrtf(dx * dx + dy * dy);
    } else if (i == 0) {
      distance = distance_to_track(train_a, next_track);
    } else {
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

  float x = train->x;
  float y = train->y;

  float *new_coord = (float *)malloc(2 * sizeof(float));
  if (new_coord == NULL) {
    printf("Malloc failled\n");
    exit(EXIT_FAILURE);
  } else {
    new_coord[0] = x;
    new_coord[1] = y;
  }

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

// ???
int nb_next_critical(Track *track_list, int track_len, int track_id) {
  int counter = 0;
  while (in_track(track_list, track_len, track_list[(track_id + counter) % track_len])) {
    counter++;
  }
  return counter;
}

// Checks if a train is on a specified track
int train_on_track(Track **tracks_list, Train **trains, int trains_nb, Track track) {
  for (int i = 0; i < trains_nb; i++) {
    Train current_train = *trains[i];
    Track current_track = tracks_list[i][current_train.last_track];
    if (same_track(track, current_track)) {
      return i;
    }
  }
  return -1;
}

float new_speed(Track **tracks_list, int *tracks_len, Train **trains, int trains_nb,
                int train_id, Track *critical, int critical_len) {
  Train *train = trains[train_id];
  int track_list_id = train->track_list;
  Track *track_list = tracks_list[track_list_id];
  int track_len = tracks_len[track_list_id];

  int ntrack_id = (trains[train_id]->last_track + 1) % tracks_len[track_list_id];
  Track next_track = tracks_list[track_list_id][ntrack_id];

  int t = train_on_track(tracks_list, trains, trains_nb, next_track);
  if (t != -1) {
    printf("\tTrain ahead of train %d, slowing down\n", train_id);
    Track track_b = tracks_list[trains[t]->track_list][trains[t]->last_track];
    float d = distance_trains(*train, *trains[t], track_list, track_len, track_b);

    if (d < 0) {
      d = distance_to_track(*train, next_track);
    }

    float d_max = distance_tracks(track_list[train->last_track], track_list[ntrack_id]);
    return MAX_SPEED * fmax(0.0, (d / (4 * d_max) - 0.2));
  }
  return MAX_SPEED;
}

// Commande séquentielle des trains
float new_speed_v2(Track **tracks_list, int *tracks_len, Train **trains, int trains_nb,
                int train_id, Track *critical, int critical_len) {
  Train* train = trains[train_id];
  Track* track_list = tracks_list[train->track_list];
  int track_len = tracks_len[train->track_list];

  Track current_track = track_list[train->last_track];
  Track next_track = track_list[(train->last_track + 1) % track_len];
  Track nnext_track = track_list[(train->last_track + 2) % track_len];

  float *next_coord = simulate_movement(train, track_list, track_len, 0.1);
  float *nnext_coord = simulate_movement(train, track_list, track_len, 0.2); 

  // Initialisaiton de la vitesse max en fonction de si l'on est dans un virage
  float u_max = MAX_SPEED;
  if ((current_track.x != next_track.x) && (current_track.y != next_track.y)) {
    u_max = MAX_SPEED_TURN;
  }
  
  // Initialisation de la consigne de vitesse
  float u_speed = 0.0;

  // Si l'on va rentrer dans une section critique
  if (in_track(critical, critical_len, next_track)) {
    int crit_index = track_index(critical, critical_len, next_track);

    // Réservation de la prochaine section critique
    if (critical[crit_index].available) {
      critical[crit_index].available = false;
      critical[crit_index].r_id = train_id;
      for (int i = 1 + train->track_list; in_track(critical, critical_len, track_list[i % track_len]); i++) {
        int crit_index = track_index(critical, critical_len, track_list[i % track_len]);
        if (critical[crit_index].available) {
          critical[crit_index].available = false;
          critical[crit_index].r_id = train_id;
        } else {
          printf("This shouldn't happen...");
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
      return 0;
    }
  }
  
  // Gestion de la distance inter trains
  for (int i = 0; i < trains_nb; i++) {
    float d = distance_trains(*train, *trains[i], track_list, track_len,
                              tracks_list[trains[i]->track_list][trains[i]->last_track]);
    if (d > 0) {
      return 0.0;
    }
  }

  // Si l'on sort d'une section critique
  return 0.0;
}

// TODO(jdumezy) separate critical sections to consider them as "one big track"
// then, use a mutex ? it does not really make sense, although it does, because we do not have to treat trains as treads
// toggle the availability of all tracks of the critical section in the critical list
// trains have to look ahead --> 2 times the times they need to decellerate
// sigmoid for u_speed
// go with mobile sections otherwise
