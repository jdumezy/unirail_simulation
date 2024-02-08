#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "logic.h"
#include "plot.h"

float scale_x(int x) {
    float xf = (float)x;
    return MARGIN + ((xf - 1.0) / (MAX_X - 1.0)) * (WIDTH - 2 * MARGIN);
}

float scale_y(int y) {
    float yf = (float)y;
    return MARGIN + ((yf - 1.0) / (MAX_Y - 1.0)) * (HEIGHT - 2 * MARGIN);
}

float scale_xf(float xf) {
    return MARGIN + ((xf - 1.0) / (MAX_X - 1.0)) * (WIDTH - 2 * MARGIN);
}

float scale_yf(float yf) {
    return MARGIN + ((yf - 1.0) / (MAX_Y - 1.0)) * (HEIGHT - 2 * MARGIN);
}

void draw_tracks(SDL_Renderer *renderer, Color color, Track *track_list, int track_len) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

  for (int i = 0; i < track_len; i++) {
    Track current_track = track_list[i];
    Track next_track = track_list[(i + 1) % track_len];
    
    int x_current = scale_x(current_track.x);
    int y_current = scale_y(current_track.y);
    int x_next = scale_x(next_track.x);
    int y_next = scale_y(next_track.y);
    
    SDL_RenderDrawLine(renderer, x_current, y_current, x_next, y_next);
  }
}

void draw_train(SDL_Renderer *renderer, Color color, Train *train, Track *track_list, int track_len) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  calculate_next_position(train, track_list, track_len);
  float x = scale_xf(train->x);
  float y = scale_yf(train->y);

  SDL_Rect train_rect = {
    .x = x-TRAIN_SIZE,
    .y = y-TRAIN_SIZE,
    .w = 2*TRAIN_SIZE,
    .h = 2*TRAIN_SIZE
  };

  SDL_RenderFillRect(renderer, &train_rect);
}

