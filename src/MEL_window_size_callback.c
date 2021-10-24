#include "../include/MEL_opengl.h"

void window_size_callback(GLFWwindow * window, int width, int height){
	glViewport(0, 0, width, height);
}
