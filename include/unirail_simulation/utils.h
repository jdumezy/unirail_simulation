#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// In ms
#define RESPONSE_TIME 200.0
#define TIME_STEP 1.0
#define MAX_SPEED 0.008
#define MAX_SPEED_TURN 0.005


#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

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
  int r_id;
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
Track* diff_track(Track *track_list_a, int track_len_a,
                  Track *track_list_b, int track_len_b,
                  int *diff_size);
int idx(int index, int max);

#endif /* UTILS_H */

