#include "../include/MEL_misc.h"

/* Delta Time */
static const double MEL_delta_time_high = 0.1;
static double       MEL_last_frame_time;
static double       MEL_current_frame_time;
double              MEL_delta_time;

void MEL_calculate_delta(){
	MEL_current_frame_time = glfwGetTime();
	MEL_delta_time = MEL_current_frame_time - MEL_last_frame_time;
	if (MEL_delta_time > MEL_delta_time_high){
		MEL_delta_time = MEL_delta_time_high;
	}
	MEL_last_frame_time = MEL_current_frame_time;
}

double MEL_delta(){
	return MEL_delta_time;
}
/* * * * * * * */

/* FPS */
static double   MEL_previous_time;
static double   MEL_current_time;
static double   MEL_time_diff;
static uint32_t MEL_fps_counter;
static uint32_t MEL_FPS;
static double   MEL_frame_time;

void MEL_calculate_FPS(){
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

uint32_t MEL_fps(){
	return MEL_FPS;
}
