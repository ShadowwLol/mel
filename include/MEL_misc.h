#ifndef _MEL_MISC_H
#define _MEL_MISC_H

#include "MEL_opengl.h"

/* Randomness */
#include <time.h>
#define randomize(){srand(time(NULL));}
#define rand_range(minv, maxv) ((rand() % (maxv - minv + 1)) + minv)
/* * * * * * * */
static clock_t startm, stopm;
#define MEL_TIMER_START(){\
	if ((startm = clock()) < 0){MEL_log(LOG_ERROR, "Failed starting timer"); return -1;}\
}

#define MEL_TIMER_END(){\
	if ((stopm = clock()) < 0){MEL_log(LOG_ERROR, "Failed ending timer"); return -1;}\
}

#define MEL_TIME_ELAPSED() (((double)stopm-startm)/CLOCKS_PER_SEC)

void MEL_calculate_fps();
uint32_t MEL_fps();

void MEL_calculate_delta();
double MEL_delta();
#endif
