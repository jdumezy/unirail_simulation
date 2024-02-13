#ifndef LOGIC_H
#define LOGIC_H

#include "utils.h"
#include <stdbool.h>

Train init_train(int id, Track *track_list, int track_len, int start);
float next_speed(float current_speed, float input_speed);
void calculate_next_position(Train *train, Track *track_list, int track_len);
bool detect_collision(Train *train_a, Train *train_b, float radius);

#endif /* LOGIC_H */

