#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
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

Track* load_track() {
    json_error_t error;
    json_t *root = json_load_file("/home/jd/Documents/dev/unirail_simulation/data/track_1.json", 0, &error);
    if (root == NULL) {
        printf("Error opening file: %s\n", error.text);
        return NULL;
    }

    if (!json_is_array(root)) {
        printf("Root element is not an array\n");
        json_decref(root);
        return NULL;
    }

    int num_points = json_array_size(root);
    Track* track_list = malloc(sizeof(Track) * num_points);
    if (track_list == NULL) {
        printf("Error allocating memory for track list\n");
        json_decref(root);
        return NULL;
    }

    for (int i = 0; i < num_points; i++) {
        json_t *point_obj = json_array_get(root, i);
        if (!json_is_object(point_obj)) {
            printf("Element at index %d is not an object\n", i);
            free(track_list);
            json_decref(root);
            return NULL;
        }

        json_t *x_val = json_object_get(point_obj, "x");
        json_t *y_val = json_object_get(point_obj, "y");
        if (!json_is_integer(x_val) || !json_is_integer(y_val)) {
            printf("Invalid x or y value for point at index %d\n", i);
            free(track_list);
            json_decref(root);
            return NULL;
        }

        int x = json_integer_value(x_val);
        int y = json_integer_value(y_val);

        Track track = { x, y, 0, true };
        track_list[i] = track;
    }

    json_decref(root);
    return track_list;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Unirail Simulation jdumezy",
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

    // Set the color to white 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int x1 = 100;
    int y1 = 100;
    int x2 = 300;
    int y2 = 400;

    // Draw a line from (x1,y1) to (x2,y2)
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    // Update the renderer
    SDL_RenderPresent(renderer);

    
    // Wait for 5 seconds before closing the window
    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

