// Copyright 2024 Jules Dumezy

#ifndef UNIRAIL_SIMULATION_INCLUDE_PLOT_H
#define UNIRAIL_SIMULATION_INCLUDE_PLOT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "utils.h"

// recommended 800 x 950
#define HEIGHT 800.
#define WIDTH 950.
#define MAX_X 17.
#define MAX_Y 14.
#define MARGIN 50.
#define TRAIN_SIZE 10.

void draw_tracks(SDL_Renderer *renderer, Color color, Track *track_list,
                 int track_len, float thickness, float w, float h);
void draw_train(SDL_Renderer *renderer, Color color, Train *train,
                Track *track_list, int track_len, float w, float h);
void draw_critical(SDL_Renderer *renderer, Color color,
                   Track *critical, int critical_len, float w, float h);
void draw_shared(SDL_Renderer *renderer, Color color, Track *shared,
                 int shared_len, float w, float h);

#endif /* UNIRAIL_SIMULATION_INCLUDE_PLOT_H */

