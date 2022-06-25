#ifndef _MEL_MISC_H
#define _MEL_MISC_H

#include "mel.h"

/* Randomness */
#include <time.h>
#define randomize(){srand(time(NULL));}
#define rand_range(minv, maxv) ((rand() % (maxv - minv + 1)) + minv)
/* * * * * * * */
static clock_t startm, stopm;
#define MEL_TIMER_START(){\
	if ((startm = clock()) < 0){mlog(LOG_ERROR, "Failed starting timer"); return -1;}\
}

#define MEL_TIMER_END(){\
	if ((stopm = clock()) < 0){mlog(LOG_ERROR, "Failed ending timer"); return -1;}\
}

#define MEL_TIME_ELAPSED() (((double)stopm-startm)/CLOCKS_PER_SEC)

/* Delta Time */
static void MEL_calculate_delta();
static double MEL_delta();

static const double MEL_delta_time_high = 0.1;
static double MEL_last_frame_time;
static double MEL_current_frame_time;
static double MEL_delta_time;

inline static void MEL_calculate_delta() {
  MEL_current_frame_time = glfwGetTime();
  MEL_delta_time = MEL_current_frame_time - MEL_last_frame_time;
  if (MEL_delta_time > MEL_delta_time_high) {
    MEL_delta_time = MEL_delta_time_high;
  }
  MEL_last_frame_time = MEL_current_frame_time;
}

inline static double MEL_delta() {
  return MEL_delta_time;
}

/* * * * * * * */

/* FPS */
static void MEL_calculate_fps();
static uint32_t MEL_fps();

static double MEL_previous_time;
static double MEL_current_time;
static double MEL_time_diff;
static uint32_t MEL_fps_counter;
static uint32_t MEL_FPS;
static double MEL_frame_time;

inline static void MEL_calculate_fps() {
  MEL_current_time = glfwGetTime();
  MEL_time_diff = MEL_current_time - MEL_previous_time;
  MEL_fps_counter++;
  if (MEL_time_diff >= 1 / 30.0) {
    MEL_FPS = (1.0 / MEL_time_diff) * MEL_fps_counter;
    MEL_frame_time = (MEL_time_diff / MEL_fps_counter) * 1000;
    MEL_previous_time = MEL_current_time;
    MEL_fps_counter = 0;
  }
}

inline static uint32_t MEL_fps() {
  return MEL_FPS;
}

/* * * */

#endif
