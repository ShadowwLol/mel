#include "../include/MEL_opengl.h" /* for gl defs */
#include "../include/MEL_io.h"     /* for i/o     */
#include "../include/MEL_def.h"    /* for configs */
#include "../include/MEL_shader.h" /* for shaders */
#include "../include/MEL_logs.h"   /* for logging */
#include "../include/MEL_image.h"  /* for images  */
#include "../include/MEL_rect.h"   /* for rects   */

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
	glfwSwapInterval(0);

	/* Setting callbacks */
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(win, key_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
	glfwSetWindowSizeLimits(win, (win_mode->width/2), (win_mode->height/2), win_mode->width, win_mode->height);
	//glfwSetWindowAspectRatio(win, ASPECT_RATIO_W, ASPECT_RATIO_H);

	glfwSetScrollCallback(win, scroll_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	/* * * * * * * * * * */

	/* Images  */
	MEL_Renderer2D Rend;
	MEL_Renderer2D_init(Rend);
	smiley = img_load_image(Rend, "resources/images/smiley.png", GL_RGBA);
	smiley.rect.pos[0] = 640.0f;
	smiley.rect.pos[1] = 360.0f;
	Image crate = img_load_image(Rend, "resources/images/container.jpg", GL_RGB);
	MEL_Rect r = MEL_load_rect(Rend);
	r.color[0] = 1.0f;
	r.color[1] = 0.0f;
	r.color[2] = 0.0f;
	r.size[0] = WINDOW_WIDTH;
	r.size[1] = WINDOW_HEIGHT;

	crate.rect.size[0] /= 2;
	crate.rect.size[1] /= 2;

	crate.rect.src[0] = 750/2;
	crate.rect.src[1] = (crate.rect.size[1])/2;
	crate.rect.src[2] = 200/2;
	crate.rect.src[3] = crate.rect.size[1]/2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double MEL_prevt = 0.0f;
	double MEL_currt = 0.0f;
	double MEL_tdiff = 0.0f;
	uint32_t MEL_cter = 0;

	/* Main loop */
	while (!glfwWindowShouldClose(win)){
		glfwPollEvents();
		glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
		glClear(GL_COLOR_BUFFER_BIT);
		smiley.rect.src[2] = smiley.rect.size[0];
		smiley.rect.src[3] = smiley.rect.size[1];

		/* Drawing */
		MEL_update_rect(Rend, r, MEL_IMAGE_STATIC);
		MEL_update_image(Rend, crate, MEL_IMAGE_STATIC);
		MEL_update_image(Rend, smiley, MEL_IMAGE_DYNAMIC);

		/* * * * * */
		MEL_currt = glfwGetTime();
		MEL_tdiff = MEL_currt - MEL_prevt;
		++MEL_cter;
		if (MEL_tdiff >= (1.0f / 30.0f)){
			double MEL_fps = (1.0f/MEL_tdiff) * MEL_cter;
			MEL_prevt = MEL_currt;
			MEL_cter = 0;
			printf("%ffps\n", MEL_fps);
		}

		glfwSwapBuffers(win);
	}

		/* * * * * * */

		/* Terminating */
		MEL_destroy_image(smiley);
		MEL_destroy_image(crate);
		MEL_Renderer2D_destroy(Rend);
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
						if (smiley.rect.color[0] == 1.0f){
							smiley.rect.color[0] = 0.0f;
						}else{
							smiley.rect.color[0] = 1.0;
						}
						break;
					default:
						break;
				}
				break;
			case GLFW_REPEAT:
				switch(key){
					case GLFW_KEY_W:
						smiley.rect.pos[1] -= 10;
						break;
					case GLFW_KEY_A:
						smiley.rect.pos[0] -= 10;
						break;
					case GLFW_KEY_S:
						smiley.rect.pos[1] += 10;
						break;
					case GLFW_KEY_D:
						smiley.rect.pos[0] += 10;
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
		glViewport(0, 0, width, height);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
		//printf("%lf, %lf", xoffset, yoffset);
		switch((int)yoffset){
			case -1:
				smiley.rect.size[0] -= 10;
				smiley.rect.size[1] -= 10;
				break;
			case 1:
				smiley.rect.size[0] += 10;
				smiley.rect.size[1] += 10;
				break;
			default:
				break;
		}
	}
