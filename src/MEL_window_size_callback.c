#include "../include/MEL_opengl.h"
#include "../include/MEL_def.h"
#include "../include/MEL_Renderer2D.h"

extern MEL_Window win;
extern MEL_Renderer2D Rend;

/**
 * @brief [
 *         Window resize callback:
 *         Maintains viewport aspect ratio and allows window resizing
 *        ]
 *
 * @param window [GLFW window]
 * @param width  [The new width]
 * @param height [The new height]
 */
void window_size_callback(GLFWwindow * window, int width, int height){
	float t_w = width;
	float t_h = (width/(WINDOW_WIDTH/WINDOW_HEIGHT));
	if (t_h > height){
		t_h = height;
		t_w = (t_h * (WINDOW_WIDTH/WINDOW_HEIGHT));
	}

	float padx = ((width - t_w)/2.0f);
	float pady = ((height - t_h)/2.0f);
	glViewport(padx, pady, t_w, t_h);
	glm_ortho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1.0f, 1.0f, Rend.projection);
}
