#include "../include/MEL_opengl.h"
#include "../include/MEL_io.h"
#include "../include/MEL_def.h"
#include "../include/MEL_shader.h"
#include <cglm/cam.h>
#include "../include/MEL_logs.h"
#include "../include/MEL_image.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
void window_size_callback(GLFWwindow * window, int width, int height);
int toggleFullscreen(GLFWwindow * window, GLFWvidmode * win_mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static GLFWwindow * win;
static GLFWvidmode* win_mode;
static Image smiley;

#if __WIN32
#include <windows.h>
int main(void){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
#else
int main(void){
#endif
	/* Initializing the library  */
	if (!glfwInit()){
		log_log(LOG_ERROR, "Failed initializing GLFW");
		return -1;
	}else{
		log_log(LOG_SUCCESS, "Successfully initialized GLFW");
	}
	/* * * * * * * * * * * * * * */

	/* Window hints  */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	/* * * * * * * * * * * */

	/* Creating the window */
	win_mode = (GLFWvidmode *)glfwGetVideoMode(glfwGetPrimaryMonitor());
	win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, glfwGetPrimaryMonitor(), NULL);
	if (!win){
		log_log(LOG_ERROR, "Failed creating window");
		glfwTerminate();
		return -1;
	}else{
		log_log(LOG_SUCCESS, "Successfully created window");
	}
	/* * * * * * * * * * * */

	/* Make the window's context current */
	glfwMakeContextCurrent(win);
	/* * * * * * * * * * * * * * * * * * */

	/* OPENGL configuration */
	gladLoadGL();
	glfwSwapInterval(1);

	/* Setting callbacks */
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(win, key_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
	//glfwSetWindowAspectRatio(window.window, 16, 9);
	glfwSetWindowSizeLimits(win, (win_mode->width/2), (win_mode->height/2), win_mode->width, win_mode->height);
	glfwSetWindowAspectRatio(win, ASPECT_RATIO_W, ASPECT_RATIO_H);

	glfwSetScrollCallback(win, scroll_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	/* * * * * * * * * * */

	/* Images  */
	MEL_Renderer2D Rend;
	MEL_Renderer2D_init(Rend);
	smiley = img_load_image("resources/images/smiley.png", GL_RGBA, 640.0f, 360.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	MEL_prepare_image(Rend, smiley);
	Image crate = img_load_image("resources/images/container.jpg", GL_RGB, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	MEL_prepare_image(Rend, crate);
	crate.width /= 2;
	crate.height /= 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Main loop */
	while (!glfwWindowShouldClose(win)){
		glfwPollEvents();
		glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
		glClear(GL_COLOR_BUFFER_BIT);

		/* Drawing */
		crate.rect.x = 0;
		crate.rect.y = 0;
		MEL_update_image(Rend, crate);
		MEL_update_image(Rend, smiley);
		/* * * * * */

		glfwSwapBuffers(win);

	}
	/* * * * * * */

	/* Terminating */
	MEL_delete_image(smiley);
	MEL_delete_image(crate);
	glfwDestroyWindow(win);
	glfwTerminate();
	/* * * * * * * */

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	switch(action){
		case GLFW_RELEASE:
			switch(key){
				case GLFW_KEY_ESCAPE:
					/* Close the game */
					glfwSetWindowShouldClose(win, GLFW_TRUE);
					break;
				case GLFW_KEY_F11:
					/* Fullscreen the game */
					toggleFullscreen(window, win_mode);
					break;
				case GLFW_KEY_SPACE:
					if (smiley.rect.R == 1.0f){
						smiley.rect.R = 0.0f;
					}else{
						smiley.rect.R = 1.0;
					}
					break;
				default:
					break;
			}
			break;
		case GLFW_REPEAT:
			switch(key){
				case GLFW_KEY_W:
					smiley.rect.y -= 10;
					break;
				case GLFW_KEY_A:
					smiley.rect.x -= 10;
					break;
				case GLFW_KEY_S:
					smiley.rect.y += 10;
					break;
				case GLFW_KEY_D:
					smiley.rect.x += 10;
					break;
				case GLFW_KEY_RIGHT:
					smiley.rect.rotation += 1.0f;
					break;
				case GLFW_KEY_LEFT:
					smiley.rect.rotation -= 1.0f;
					break;
				default:
					break;
			}
		default:
			break;
	}
}

void error_callback(int error, const char* description){
	fprintf(stderr, "[-] Error: %s\n", description);
}

void window_size_callback(GLFWwindow * window, int width, int height){
	/*
	float aspectRatio = (float)width/(float)height;
	printf("Window size %fx%f, aspect ratio: %f\n", WINDOW_WIDTH, WINDOW_HEIGHT, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT);
	if(aspectRatio != ASPECT_RATIO) {
		if(aspectRatio > ASPECT_RATIO) {
			height = (1.f / ASPECT_RATIO) * width;
		}else {
			width = ASPECT_RATIO * height;
		}
	}
	glViewport(0, 0, width, height);
	printf("Setting window size to %dx%d, aspect ratio: %f\n", width, height, (float)width/(float)height);
	*/
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	//printf("%lf, %lf", xoffset, yoffset);
	switch((int)yoffset){
		case -1:
			smiley.width -= 10;
			smiley.height -= 10;
			break;
		case 1:
			smiley.width += 10;
			smiley.height += 10;
			break;
		default:
			break;
	}
}
