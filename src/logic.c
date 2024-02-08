#include "logic.h"
#include <math.h>

Train init_train(int id, Track *track_list, int track_len, int start) {
  int x = track_list[start].x;
  int y =  track_list[start].y;
  Train train = { x, y, 0.1, id, start };
  return train;
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
  float distance = train->speed;

  float x = train->x;
  float y = train->y;

  float d_track = distance_to_track((*train), track_list[(track_id + 1) % track_len]);
  
  while (d_track < distance) {
    distance -= d_track;
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
  train->x += unit_x * distance;
  train->y += unit_y * distance;
  
  train->last_track = track_id;
}

bool detect_collision(Train *train_a, Train *train_b, float radius) {
  float dx = fabs(train_a->x - train_b->x);
  float dy = fabs(train_a->y - train_b->y);
  if ((dx < radius) && (dy < radius)) {
    return true;
  }
  return false;  
}
