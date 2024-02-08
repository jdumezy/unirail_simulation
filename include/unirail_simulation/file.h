#ifndef FILE_H
#define FILE_H

#include "logic.h"

int line_number(const char *filename);
Track* load_track(const char *filename);

#endif /* FILE_H */
