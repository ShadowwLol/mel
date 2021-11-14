#ifndef _MEL_MISC_H
#define _MEL_MISC_H

#include "../include/MEL_def.h"

/* Randomness */
#include <time.h>
#define randomize(){srand(time(NULL));}
#define rand_range(minv, maxv) ((rand() % (maxv - minv + 1)) + minv)
/* * * * * * * */

void MEL_calculate_fps();
uint32_t MEL_fps();

void MEL_calculate_delta();
double MEL_delta();
#endif
