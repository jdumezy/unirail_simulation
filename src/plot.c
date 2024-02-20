// Copyright 2024 Jules Dumezy

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "utils.h"
#include "plot.h"

// Facteur d'échelle sur x
float scale_x(int x, float w) {
    float xf = (float)x;
    return MARGIN + ((xf - 1.0) / (MAX_X - 1.0)) * (w - 2 * MARGIN);
}

// Facteur d'échelle sur y
float scale_y(int y, float h) {
    float yf = (float)y;
    return MARGIN + ((yf - 1.0) / (MAX_Y - 1.0)) * (h - 2 * MARGIN);
}

// Facteur d'échelle sur x (float)
float scale_xf(float xf, float w) {
    return MARGIN + ((xf - 1.0) / (MAX_X - 1.0)) * (w - 2 * MARGIN);
}

// Facteur d'échelle sur y (float)
float scale_yf(float yf, float h) {
    return MARGIN + ((yf - 1.0) / (MAX_Y - 1.0)) * (h - 2 * MARGIN);
}

void draw_thick_line(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, int thickness) {
    if (x1 == x2 || y1 == y2) {
        int rectX = x1 < x2 ? x1 : x2;
        int rectY = y1 < y2 ? y1 : y2;
        int rectW = x1 == x2 ? thickness : fabs(x2 - x1);
        int rectH = y1 == y2 ? thickness : fabs(y2 - y1);

        if (x1 == x2) {
            rectX -= thickness / 2;
        }

        if (y1 == y2) {
            rectY -= thickness / 2;
        }

        SDL_Rect lineRect = {rectX, rectY, rectW, rectH};
        SDL_RenderFillRect(renderer, &lineRect);
    } else {
        float dx = x2 - x1;
        float dy = y2 - y1;
        float distance = sqrtf(dx * dx + dy * dy);

        dx /= distance;
        dy /= distance;

        float step = thickness / 2.0f;

        int numSquares = (int)(distance / step) + 1;

        for (int i = 0; i < numSquares; i++) {
            float cx = x1 + dx * (i * step);
            float cy = y1 + dy * (i * step);

            float sx = cx - thickness / 2.0f;
            float sy = cy - thickness / 2.0f;

            SDL_Rect square = {(int)sx, (int)sy, thickness, thickness};

            SDL_RenderFillRect(renderer, &square);
        }
    }
}

// Affichages des circuits
void draw_tracks(SDL_Renderer *renderer, Color color, Track *track_list,
                 int track_len, float thickness, float w, float h) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

  for (int i = 0; i < track_len; i++) {
    Track current_track = track_list[i];
    Track next_track = track_list[(i + 1) % track_len];

    int x_current = scale_x(current_track.x, w);
    int y_current = scale_y(current_track.y, h);
    int x_next = scale_x(next_track.x, w);
    int y_next = scale_y(next_track.y, h);
    
    if (thickness < 0.5) {
      SDL_RenderDrawLine(renderer, x_current, y_current, x_next, y_next);
    } else {
      draw_thick_line(renderer, x_current, y_current, x_next, y_next, thickness);
    }
  }
}


void draw_train_icon(SDL_Renderer* renderer, float x, float y, float width, float height) {
    // Main body of the train
    SDL_Rect body = {x, y + height / 4, width, height / 2};
    SDL_RenderFillRect(renderer, &body);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Cabin
    SDL_Rect cabin = {x + width - (width / 3), y, width / 4, height / 4};
    SDL_RenderFillRect(renderer, &cabin);

    // Wheels
    int wheelWidth = width / 6;
    int wheelHeight = height / 4;
    SDL_Rect wheel1 = {x + (width / 10), y + (3 * height / 4), wheelWidth, wheelHeight};
    SDL_RenderFillRect(renderer, &wheel1);

    SDL_Rect wheel2 = {x + width - (2 * wheelWidth), y + (3 * height / 4), wheelWidth, wheelHeight};
    SDL_RenderFillRect(renderer, &wheel2);
}

// Affichage des trains
void draw_train(SDL_Renderer *renderer, Color color, Train *train,
                Track *track_list, int track_len, float w, float h) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  float x = scale_xf(train->x, w);
  float y = scale_yf(train->y, h);

  SDL_Rect train_rect = {
    .x = x-TRAIN_SIZE,
    .y = y-TRAIN_SIZE,
    .w = 2*TRAIN_SIZE,
    .h = 2*TRAIN_SIZE
  };

  //SDL_RenderFillRect(renderer, &train_rect);
  draw_train_icon(renderer, train_rect.x, train_rect.y, train_rect.w, train_rect.h);

}

// Affichage des sections critiques
void draw_critical(SDL_Renderer *renderer, Color color, Track *critical,
                   int critical_len, float w, float h) {
  for (int i = 0; i < critical_len; i++) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    
    float x = scale_xf(critical[i].x, w);
    float y = scale_yf(critical[i].y, h);

    SDL_Rect border = {
      .x = x-(TRAIN_SIZE)/1.5,
      .y = y-(TRAIN_SIZE)/1.5,
      .w = 2*(TRAIN_SIZE)/1.5,
      .h = 2*(TRAIN_SIZE)/1.5
    };

    SDL_RenderFillRect(renderer, &border);


    if (critical[i].available) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, color.r/4, color.g/4, color.b/4, 255);
    }
    SDL_Rect rect = {
      .x = 2+x-(TRAIN_SIZE)/1.5,
      .y = 2+y-(TRAIN_SIZE)/1.5,
      .w = 2*(TRAIN_SIZE)/1.5-4,
      .h = 2*(TRAIN_SIZE)/1.5-4
    };

    SDL_RenderFillRect(renderer, &rect);
  }
}


// Affichage des sections partagées
void draw_shared(SDL_Renderer *renderer, Color color, Track *shared,
                 int shared_len, float w, float h) {
  for (int i = 0; i < shared_len; i++) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    
    float x = scale_xf(shared[i].x, w);
    float y = scale_yf(shared[i].y, h);

    SDL_Rect border = {
      .x = x-(TRAIN_SIZE)/1.5,
      .y = y-(TRAIN_SIZE)/1.5,
      .w = 2*(TRAIN_SIZE)/1.5,
      .h = 2*(TRAIN_SIZE)/1.5
    };

    SDL_RenderFillRect(renderer, &border);


    if (shared[i].available) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, color.r/4, color.g/4, color.b/4, 255);
    }
    SDL_Rect rect = {
      .x = 2+x-(TRAIN_SIZE)/1.5,
      .y = 2+y-(TRAIN_SIZE)/1.5,
      .w = 2*(TRAIN_SIZE)/1.5-4,
      .h = 2*(TRAIN_SIZE)/1.5-4
    };

    SDL_RenderFillRect(renderer, &rect);
  }
}

