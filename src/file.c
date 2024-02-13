#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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

