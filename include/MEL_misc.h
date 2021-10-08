#ifndef MISC_H
#define MISC_H

#ifdef MISC_RANDOM
#include <stdlib.h>
#include <time.h>
#define randomize(){srand(time(NULL));}
#define rand_range(minv, maxv) ((rand() % (maxv - minv + 1)) + minv)
#endif

#endif