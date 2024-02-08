#include "logic.h"
#include <stdio.h>

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
void calculate_next_position(Train *train, float distance, Track *track_list, int track_len) {
  int track_id = train->last_track;

  float x = train->x;
  float y = train->y;

  // distance jusqu'au prochain point
  float d_track = distance_to_track((*train), track_list[(track_id + 1) % track_len]);
  
  // si on dépasse le prochain point
  while (d_track < distance) {
    // distance  parcourir sur la nouvelle section
    distance -= d_track;
    track_id = (track_id + 1) % track_len;
    
    x = (float)track_list[track_id].x;
    y = (float)track_list[track_id].y;

    // calcul de la nouvelle distance entre le prochain point et celui d'après
    d_track = distance_tracks(track_list[track_id], track_list[track_id + 1]);
  }

  // on récupère les deux rails entre lesquels on va se déplacer
  Track currrent_track = track_list[track_id];
  Track next_track = track_list[track_id + 1];


  // on récupère les coordonées
  float x_next = (float)next_track.x;
  float y_next = (float)next_track.y;

  float dx = x_next - x;
  float dy = y_next - y;

  float unit_x = dx / d_track;
  float unit_y = dy/ d_track;
  train->x += unit_x * distance;
  train->y += unit_y * distance;
  
  printf("x: %f y; %f\n", train->x, train->y);

  train->last_track = track_id;
}


// initialize tracks




