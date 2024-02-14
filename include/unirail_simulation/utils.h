#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// In ms
#define RESPONSE_TIME 200.0
#define TIME_STEP 1.0
#define MAX_SPEED 0.01

typedef struct {
  int r;
  int g;
  int b;
} Color;

typedef struct {
  int x;
  int y;
  int section;
  bool available;
} Track;

typedef struct {
  float x;
  float y;
  float speed;
  float u_speed;
  int id;
  int track_list;
  int last_track;
} Train;

bool in_track(Track *track_list, int track_len, Track track);
int track_index(Track *track_list, int track_len, Track track);
bool same_track(Track track_a, Track track_b);

#endif /* UTILS_H */

