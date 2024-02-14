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

