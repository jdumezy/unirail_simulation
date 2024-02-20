// Copyright 2024 Jules Dumezy

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
  // Récupération de la durée de simulation
  int duration = 10;
  if (argc > 1) {
    duration = atoi(argv[1]);
  }
  
  // For fast simulations
  bool unlimited = false;
  if (argc > 2) {
    if (strcmp(argv[2], "-u") == 0) {
      unlimited = true;
    }
  }

  float g_windowWidth = 950.; // Initial width
  float g_windowHeight = 800.; // Initial height

  // Création de la fenêtre
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Unirail Simlulation jdumezy",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    g_windowWidth, g_windowHeight,
    SDL_WINDOW_RESIZABLE
  );

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
  }

  // Initialisation des la simulation    
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Event event;

  // Chemins à adapter
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
  Track *critical = critical_sections_man(&critical_len);

  int shared_len_temp = 0;
  Track *shared_temp = critical_sections(tracks_list, tracks_len, 3, &shared_len_temp);
  int shared_len = 0;
  Track *shared = diff_track(shared_temp, shared_len_temp, critical,
                             critical_len, &shared_len);
  free(shared_temp);
  
  // Colors for Trains
  Color train_1_color = {255, 165, 0};     // Orange
  Color train_2_color = {85, 255, 85};  // Light Green
  Color train_3_color = {85, 85, 255};  // Light Blue

  // Colors for Sections
  Color critical_color = {255, 85, 85};  // Light Red
  Color shared_color = {255, 255, 85};  // Yellow

  Train train_1 = init_train(0, track_list_1, 0, 0);
  Train train_2 = init_train(1, track_list_2, 1, 10);
  Train train_3 = init_train(2, track_list_3, 2, 4);
  
  Train *trains[] = {&train_1, &train_2, &train_3};

  float radius = 0.4;

  SDL_RenderPresent(renderer);
  
  int steps = duration * TIME_STEP * 1000;
  int isRunning = 1;
  int laps = 0;

  // Boucle itérative principale
  while (isRunning && (laps < steps || duration < 0)){
    // Logique
    train_1.u_speed = new_speed(tracks_list, tracks_len, trains, 3, 0,
                                   critical, critical_len, shared, shared_len);
    train_2.u_speed = new_speed(tracks_list, tracks_len, trains, 3, 1,
                                   critical, critical_len, shared, shared_len);
    train_3.u_speed = new_speed(tracks_list, tracks_len, trains, 3, 2,
                                   critical, critical_len, shared, shared_len);

    calculate_next_position(&train_1, track_list_1, track_len_1);
    calculate_next_position(&train_2, track_list_2, track_len_2);
    calculate_next_position(&train_3, track_list_3, track_len_3);

    // Détection collisions
    if (detect_collision(&train_1, &train_2, radius)) {
      printf("Collision between train 1 and train 2!\n");
      break;
    }
    if (detect_collision(&train_3, &train_2, radius)) {
      printf("Collision between train 2 and train 3!\n");
      break;
    }
    if (detect_collision(&train_1, &train_3, radius)) {
      printf("Collision between train 1 and train 3!\n");
      break;
    }

    // Affichage
    if (!(unlimited)) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      draw_tracks(renderer, train_1_color, track_list_1, track_len_1, 0.0, g_windowWidth, g_windowHeight);
      draw_tracks(renderer, train_2_color, track_list_2, track_len_2, 0.0, g_windowWidth, g_windowHeight);
      draw_tracks(renderer, train_3_color, track_list_3, track_len_3, 0.0, g_windowWidth, g_windowHeight);
      
      draw_critical(renderer, critical_color, critical, critical_len, g_windowWidth, g_windowHeight);
      draw_shared(renderer, shared_color, shared, shared_len, g_windowWidth, g_windowHeight);

      draw_train(renderer, train_1_color, &train_1, track_list_1, track_len_1, g_windowWidth, g_windowHeight);
      draw_train(renderer, train_2_color, &train_2, track_list_2, track_len_2, g_windowWidth, g_windowHeight);
      draw_train(renderer, train_3_color, &train_3, track_list_3, track_len_3, g_windowWidth, g_windowHeight);
      
      SDL_RenderPresent(renderer);
    }

    // Check event
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          isRunning = 0;
          break;
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            g_windowWidth = event.window.data1;
            g_windowHeight = event.window.data2;
          }
      }
    }
    
    if (!(unlimited)) {
      SDL_Delay(TIME_STEP);
    } else if (laps % 10000 == 0) {
      printf("Iteration: %d\n", laps);
    }

    laps++;
  }
  
  if (isRunning == 1) {
    // Délai avant fermeture
    SDL_Delay(5000);
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
    
  free(track_list_1);
  free(track_list_2);
  free(track_list_3);
  free(critical);
  free(shared);
   
  return 0;
}

