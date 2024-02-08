#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

#define HEIGHT 800
#define WIDTH 950
#define MAX_X 17
#define MAX_Y 14
#define MARGIN 50

float scale_x(int x) {
  float xf = (float)x;
  return MARGIN + ((xf-1)/(MAX_X-1))*(WIDTH - 2 * MARGIN);
}
float scale_y(int y) {
  float yf = (float)y;
  return MARGIN + ((yf-1)/(MAX_Y-1))*(HEIGHT - 2 * MARGIN);
}

void draw_tracks(SDL_Renderer *renderer, Color color, Track *track_list, int track_len) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

  for (int i = 0; i < track_len; i++) {
    Track current_track = track_list[i];
    Track next_track = track_list[(i + 1) % track_len];
    
    float x_current = scale_x(current_track.x);
    float y_current = scale_y(current_track.y);
    float x_next = scale_x(next_track.x);
    float y_next = scale_y(next_track.y);
    
    SDL_RenderDrawLine(renderer, x_current, y_current, x_next, y_next);
  }
}

int line_number(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file.\n");
    return -1;
  }
    
  int nb_lines = 0;
  char buffer[512];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        nb_lines++;
    }
    
  fclose(file);
  return nb_lines;
}

Track* load_track(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file.\n");
    return NULL;
  }
  
  int x, y;
  char buffer[512];

  int nb_lines = line_number(filename);
  Track* track_list = malloc(sizeof(Track) * nb_lines);
  
  int i = 0;

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    sscanf(buffer, "%d,%d", &x, &y);
    Track track = { x, y, 0, true };
    track_list[i] = track;
    i++;
    }
    
  fclose(file);
  
  return track_list;
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Unirail Simlulation jdumezy",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    WIDTH, HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
  const char *filename_1 = "/home/jd/Documents/dev/unirail_simulation/data/track_1.csv";
  const char *filename_2 = "/home/jd/Documents/dev/unirail_simulation/data/track_2.csv"; 
  const char *filename_3 = "/home/jd/Documents/dev/unirail_simulation/data/track_3.csv";

  int track_len_1 = line_number(filename_1);
  Track *track_list_1 = load_track(filename_1);
  int track_len_2 = line_number(filename_2);
  Track *track_list_2 = load_track(filename_2);
  int track_len_3 = line_number(filename_3);
  Track *track_list_3 = load_track(filename_3);


  Color white = { 255, 255, 255};
  Color blue = { 100, 100, 255};
  Color green = { 100, 255, 100};

  draw_tracks(renderer, white, track_list_1, track_len_1);
  draw_tracks(renderer, blue, track_list_2, track_len_2);
  draw_tracks(renderer, green, track_list_3, track_len_3);

  SDL_RenderPresent(renderer);

  SDL_Delay(5000);

  SDL_DestroyWindow(window);
  SDL_Quit();
    
  free(track_list_1);
  free(track_list_2);
  free(track_list_3);
    
  return 0;
}

