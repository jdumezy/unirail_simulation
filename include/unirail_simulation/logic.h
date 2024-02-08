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
