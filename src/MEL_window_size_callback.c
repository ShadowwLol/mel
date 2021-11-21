#include "../include/MEL_opengl.h"
#include "../include/MEL_def.h"
#include "../include/MEL_render.h"

extern MEL_Window win;
extern MEL_Renderer2D Rend;

void window_size_callback(GLFWwindow * window, int width, int height){
	glViewport(0, 0, width, height);
	glm_ortho(0.0f, (float)win.mode->width, (float)win.mode->height, 0.0f, -1.0f, 1.0f, Rend.projection);
}
