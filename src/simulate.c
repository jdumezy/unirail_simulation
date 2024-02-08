#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

#define HEIGHT = 1500
#define WIDTH = 1800

void draw_tracks(SDL_Renderer *renderer, Color color, Track *track_list, int track_len) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  for (int i = 0; i < track_len; i++) {
    Track currrent_track = track_list[i];
    Track next_track = track_list[(i + 1) % track_len];

    int x_current = currrent_track.x;
    int y_current = currrent_track.y;
    int x_next = next_track.x;
    int y_next = next_track.y;

    SDL_RenderDrawLine(renderer, x_current, y_current, x_next, y_next);
    SDL_RenderPresent(renderer);
  }
}


void processFile(const char *filename) {
        int x, y;
    int lineCount = 0;
        
    printf("Number of lines: %d\n", lineCount);
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
        1920, 1080,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    const char *filename = "data/track_1.csv";
    
    int track_len = line_number(filename);
    Track *track_list = load_track(filename);
    
    SDL_RenderPresent(renderer);

    
    // Wait for 5 seconds before closing the window
    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();
    
    free(track_list);
    
    return 0;
}

