#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "logic.h"
#include "file.h"
#include "plot.h"


int main(int argc, char* argv[]) {
  int duration = 10;
  if (argc > 0) {
    duration = atoi(argv[1]);
  }

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
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  const char *filename_1 = "/home/jd/Documents/dev/unirail_simulation/data/track_1.csv";
  const char *filename_2 = "/home/jd/Documents/dev/unirail_simulation/data/track_2.csv"; 
  const char *filename_3 = "/home/jd/Documents/dev/unirail_simulation/data/track_3.csv";

  int track_len_1 = line_number(filename_1);
  Track *track_list_1 = load_track(filename_1);
  int track_len_2 = line_number(filename_2);
  Track *track_list_2 = load_track(filename_2);
  int track_len_3 = line_number(filename_3);
  Track *track_list_3 = load_track(filename_3);

  Track *tracks_list[] = {track_list_1, track_list_2, track_list_3};
  int tracks_len[] = {track_len_1, track_len_2, track_len_3};

  int critical_len = 0;
  Track *critical = critical_sections(tracks_list, tracks_len, 3, &critical_len);
 
  Color white = { 255, 255, 255 };
  Color blue = { 100, 100, 255 };
  Color green = { 100, 255, 100 };
  Color red = { 255, 0, 0};

  Train train_1 = init_train(0, track_list_1, 0, 0);
  Train train_2 = init_train(1, track_list_2, 1, 10);
  Train train_3 = init_train(2, track_list_3, 2, 4);
  
  Train *trains[] = {&train_1, &train_2, &train_3};

  float radius = 0.4;

  SDL_RenderPresent(renderer);
  
  int steps = duration * TIME_STEP * 1000;

  for (int laps = 0; laps < steps; laps++) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    train_1.u_speed = new_speed(tracks_list, tracks_len, trains, 3, 0, critical, critical_len);
    train_2.u_speed = new_speed(tracks_list, tracks_len, trains, 3, 1, critical, critical_len);
    train_3.u_speed = new_speed(tracks_list, tracks_len, trains, 3, 2, critical, critical_len);

    draw_tracks(renderer, white, track_list_1, track_len_1);
    draw_tracks(renderer, blue, track_list_2, track_len_2);
    draw_tracks(renderer, green, track_list_3, track_len_3);
    // draw_tracks(renderer, red, critical, critical_len);
    draw_train(renderer, white, &train_1, track_list_1, track_len_1);
    draw_train(renderer, blue, &train_2, track_list_2, track_len_2);
    draw_train(renderer, green, &train_3, track_list_3, track_len_3);
    
    SDL_RenderPresent(renderer);

    // Collision detection
    if (detect_collision(&train_1, &train_2, radius)) {
      printf("Collision between train 1 and train 2!\n");
      //break;
    }
    if (detect_collision(&train_3, &train_2, radius)) {
      printf("Collision between train 2 and train 3!\n");
      //break;
    }
    if (detect_collision(&train_1, &train_3, radius)) {
      printf("Collision between train 1 and train 3!\n");
      //break;
    }

    SDL_Delay(TIME_STEP);
  }

  SDL_Delay(5000);
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
    
  free(track_list_1);
  free(track_list_2);
  free(track_list_3);
   
  return 0;
}

