#include "logic.h"

bool in_track(Track track, Track *track_list, int track_len) {
  for (int i = 0; i < track_len; i++) {
    if (track_list[i].x == track.x && track_list[i].y == track.y) {
      return true;
    } 
  }
  return false;
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

// on which section am I?
int location(Train train, Track *track_list, int track_len) {
  int min_dist = distance_to_track(train, track_list[0]);
  int track_id = 0;
  for (int i = 0; i < track_len; i++) {
    float d = distance_to_track(train, track_list[i]);
    if (min_dist > d) {
      min_dist = d;
      track_id = i;
    }
  }
  return track_id;
}


// next position from given speed and time interval
void calculate_next_position(Train train, float distance, Track *track_list, int track_len) {
  int track_id = train.last_track; 
  int i = 1;
  int d_track = distance_to_track(train, track_list[(track_id + i) % track_len]);

  while (d_track < distance) {
    distance -= d_track;
    i++;
    d_track = distance_to_track(train, track_list[(track_id + i) % track_len]);
  }
  
  Track currrent_track = track_list[(track_id + i - 1) % track_len];
  Track next_track = track_list[(track_id + i) % track_len];

  float x_current = (float)currrent_track.x;
  float y_current = (float)currrent_track.y;
  float x_next = (float)next_track.x;
  float y_next = (float)next_track.y;

  float t = distance_to_track(train, next_track)/distance_tracks(currrent_track, next_track);
  
  train.x = x_current + t * (x_next - x_current);
  train.y = y_current + t * (y_next - y_current);
}


// initialize tracks




