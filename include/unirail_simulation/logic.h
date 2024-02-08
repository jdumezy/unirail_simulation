#include <math.h>
#include <stdbool.h>

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
  int id;
  int last_track;
} Train;

int location(Train train, Track *track_list, int track_len);
void calculate_next_position(Train *train, float distance, Track *track_list, int track_len);

