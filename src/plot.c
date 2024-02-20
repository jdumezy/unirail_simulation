// Copyright 2024 Jules Dumezy

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "utils.h"
#include "plot.h"

// Facteur d'échelle sur x
float scale_x(int x) {
    float xf = (float)x;
    return MARGIN + ((xf - 1.0) / (MAX_X - 1.0)) * (WIDTH - 2 * MARGIN);
}

// Facteur d'échelle sur y
float scale_y(int y) {
    float yf = (float)y;
    return MARGIN + ((yf - 1.0) / (MAX_Y - 1.0)) * (HEIGHT - 2 * MARGIN);
}

// Facteur d'échelle sur x (float)
float scale_xf(float xf) {
    return MARGIN + ((xf - 1.0) / (MAX_X - 1.0)) * (WIDTH - 2 * MARGIN);
}

// Facteur d'échelle sur y (float)
float scale_yf(float yf) {
    return MARGIN + ((yf - 1.0) / (MAX_Y - 1.0)) * (HEIGHT - 2 * MARGIN);
}

// Affichages des circuits
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

// Affichage des trains
void draw_train(SDL_Renderer *renderer, Color color, Train *train, Track *track_list, int track_len) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
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

// Affichage des sections critiques
void draw_critical(SDL_Renderer *renderer, Color color, Track *critical, int critical_len) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  for (int i = 0; i < critical_len; i++) {
    if (critical[i].available) {
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, color.r/3, color.g/3, color.b/3, 255);
    }
    float x = scale_xf(critical[i].x);
    float y = scale_yf(critical[i].y);

    SDL_Rect rect = {
      .x = x-(TRAIN_SIZE)/2,
      .y = y-(TRAIN_SIZE)/2,
      .w = 2*(TRAIN_SIZE)/2,
      .h = 2*(TRAIN_SIZE)/2
    };

    SDL_RenderFillRect(renderer, &rect);
  }
}

// Affichage des sections partagées
void draw_shared(SDL_Renderer *renderer, Color color, Track *shared, int shared_len) {
  for (int i = 0; i < shared_len; i++) {
    if (shared[i].available) {
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, color.r/3, color.g/3, color.b/3, 255);
    }
    float x = scale_xf(shared[i].x);
    float y = scale_yf(shared[i].y);

    SDL_Rect rect = {
      .x = x-(TRAIN_SIZE)/2,
      .y = y-(TRAIN_SIZE)/2,
      .w = 2*(TRAIN_SIZE)/2,
      .h = 2*(TRAIN_SIZE)/2
    };

    SDL_RenderFillRect(renderer, &rect);
  }
}

