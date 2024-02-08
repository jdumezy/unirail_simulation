
#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>

// In ms
#define RESPONSE_TIME 200.0
#define TIME_STEP 10.0
#define MAX_SPEED 0.5

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
  int last_track;
} Train;

Train init_train(int id, Track *track_list, int track_len, int start);
float next_speed(float current_speed, float input_speed);
void calculate_next_position(Train *train, Track *track_list, int track_len);
bool detect_collision(Train *train_a, Train *train_b, float radius);

#endif /* LOGIC_H */

