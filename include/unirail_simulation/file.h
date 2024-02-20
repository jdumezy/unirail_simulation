// Copyright 2024 Jules Dumezy

#ifndef UNIRAIL_SIMULATION_INCLUDE_FILE_H
#define UNIRAIL_SIMULATION_INCLUDE_FILE_H

#include "logic.h"

int line_number(const char *filename);
Track* load_track(const char *filename);

#endif /* UNIRAIL_SIMULATION_INCLUDE_FILE_H */

