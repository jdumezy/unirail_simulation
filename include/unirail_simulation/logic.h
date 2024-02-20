// Copyright 2024 Jules Dumezy

#ifndef UNIRAIL_SIMULATION_INCLUDE_LOGIC_H
#define UNIRAIL_SIMULATION_INCLUDE_LOGIC_H

#include "utils.h"

#include <stdbool.h>

Train init_train(int id, Track *track_list, int track_len, int start);
float next_speed(float current_speed, float input_speed);
void calculate_next_position(Train *train, Track *track_list, int track_len);
bool detect_collision(Train *train_a, Train *train_b, float radius);
bool is_close(Track track_a, Track track_b);
Track* critical_sections(Track **tracks_list, int *tracks_len, int tracks_nb, int *size_out);
Track* critical_sections_man(int *size_out);
float new_speed(Track **tracks_list, int *tracks_len, Train **trains, int trains_nb,
                   int train_id, Track *critical, int critical_len,
                   Track *shared, int shared_len);

#endif /* UNIRAIL_SIMULATION_INCLUDE_LOGIC_H */

