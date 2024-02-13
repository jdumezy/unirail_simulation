#ifndef PLOT_H
#define PLOT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "utils.h"

#define HEIGHT 800.
#define WIDTH 950.
#define MAX_X 17.
#define MAX_Y 14.
#define MARGIN 50.
#define TRAIN_SIZE 10.

void draw_tracks(SDL_Renderer *renderer, Color color, Track *track_list, int track_len);
void draw_train(SDL_Renderer *renderer, Color color, Train *train, Track *track_list, int track_len);

#endif /* PLOT_H */
