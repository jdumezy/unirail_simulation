#include "utils.h"
#include "logic.h"
#include <stdlib.h>
#include <math.h>

Train init_train(int id, Track *track_list, int track_len, int start) {
  int x = track_list[start].x;
  int y =  track_list[start].y;
  Train train = { x, y, 0.0, 0.1, id, start };
  return train;
}

float next_speed(float speed, float u_speed) {
    if (fabs(u_speed - speed) > 0.00001) {
      float delta_v = (TIME_STEP/RESPONSE_TIME) * (u_speed - speed);
      return speed + delta_v;
    }
    return speed;
}

float distance_tracks(Track track_a, Track track_b) {
  float dx = (float)(track_a.x - track_b.x);
  float dy = (float)(track_a.y - track_b.y);
  return sqrtf(dx * dx + dy * dy);
}

float distance_to_track(Train train, Track track) {
  float dx = (float)track.x - train.x;
  float dy = (float)track.y - train.y;
  return sqrtf(dx * dx + dy * dy);
}

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

bool detect_collision(Train *train_a, Train *train_b, float radius) {
  float dx = fabs(train_a->x - train_b->x);
  float dy = fabs(train_a->y - train_b->y);
  if ((dx < radius) && (dy < radius)) {
    return true;
  }
  return false;  
}

bool is_close(Track track_a, Track track_b) {
  if ((abs(track_a.x - track_b.x) < 2) && (abs(track_a.y - track_b.y) < 2)) {
    return true;
  }
  return false;
}

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
            Track common_track = { track.x, track.y, track.section, track.available };
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

int nb_next_critical(Track *track_list, int track_len, int track_id) {
  int counter = 0;
  while (in_track(track_list, track_len, track_list[(track_id + counter) % track_len])) {
    counter++;
  }
  return counter;
}

