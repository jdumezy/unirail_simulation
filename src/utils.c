#include <stdlib.h>

#include "utils.h"

bool in_track(Track *track_list, int track_len, Track track) {
  for (int i = 0; i < track_len; i++) {
    Track current_track  =  track_list[i];
    if ((current_track.x == track.x) && (current_track.y == track.y)) {
      return true;
    }
  }
  return false;
}

int track_index(Track *track_list, int track_len, Track track) {
  for (int i = 0; i < track_len; i++) {
    Track current_track  =  track_list[i];
    if ((current_track.x == track.x) && (current_track.y == track.y)) {
      return i;
    }
  }
  return -1;
}

bool same_track(Track track_a, Track track_b) {
  return ((track_a.x == track_b.x) && (track_a.y == track_b.y));
}

Track* diff_track(Track *track_list_a, int track_len_a,
                  Track *track_list_b, int track_len_b,
                  int *diff_size) {
  Track *diff = (Track *)malloc(track_len_a * sizeof(Track));
  if (diff == NULL) {
    *diff_size = 0;
    return NULL;
  }
  
  int counter = 0;
  for (int i = 0; i < track_len_a; i++) {
    if (!in_track(track_list_b, track_len_b, track_list_a[i])) {
      diff[counter++] = track_list_a[i];
    }
  }
  
  Track *shrinked_diff = malloc(sizeof(Track) * counter);
  if (!shrinked_diff) {
    *diff_size = -1;
    return NULL;
  }
  for (int i = 0; i < counter; i++) {
    shrinked_diff[i] = diff[i];
  }

  free(diff);

  *diff_size = counter;
  return shrinked_diff;
}
