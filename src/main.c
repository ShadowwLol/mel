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

MEL_Window win;
static Image smiley;
MEL_Camera camera;

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
	win.mode = (GLFWvidmode *)glfwGetVideoMode(glfwGetPrimaryMonitor());
	win.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, glfwGetPrimaryMonitor(), NULL);
	if (!win.window){
		log_log(LOG_ERROR, "Failed creating window");
		glfwTerminate();
		return -1;
	}else{
		log_log(LOG_SUCCESS, "Successfully created window");
	}
	/* * * * * * * * * * * */

	/* Make the window's context current */
	glfwMakeContextCurrent(win.window);
	/* * * * * * * * * * * * * * * * * * */

	/* OPENGL configuration */
	gladLoadGL();
	glfwSwapInterval(0);

	/* Setting callbacks */
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(win.window, key_callback);
	glfwSetWindowSizeCallback(win.window, window_size_callback);
	glfwSetWindowSizeLimits(win.window, (win.mode->width/2), (win.mode->height/2), win.mode->width, win.mode->height);
	//glfwSetWindowAspectRatio(win, ASPECT_RATIO_W, ASPECT_RATIO_H);

	glfwSetScrollCallback(win.window, scroll_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	/* * * * * * * * * * */

	/* Images  */
	MEL_Renderer2D Rend = MEL_Renderer2D_init();
	smiley = img_load_image(Rend, "resources/images/smiley.png", GL_RGBA);
	smiley.rect.pos[0] = 640.0f;
	smiley.rect.pos[1] = 360.0f;
	smiley.rect.color[3] = 0.5f;
	Image crate = img_load_image(Rend, "resources/images/container.jpg", GL_RGB);
	MEL_Rect r = MEL_load_rect(Rend);
	r.color[0] = 1.0f;
	r.color[1] = 0.0f;
	r.color[2] = 0.0f;
	r.color[3] = 1.0f;
	r.pos[0] = 200.0f;
	r.pos[1] = 70.0f;
	r.size[0] = 500.0f;
	r.size[1] = 500.0f;

	crate.rect.size[0] /= 2;
	crate.rect.size[1] /= 2;

	crate.rect.src[0] = 750/2.0f;
	crate.rect.src[1] = (crate.rect.size[1])/2;
	crate.rect.src[2] = 200/2.0f;
	crate.rect.src[3] = crate.rect.size[1]/2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double MEL_prevt = 0.0f;
	double MEL_currt = 0.0f;
	double MEL_tdiff = 0.0f;
	uint32_t MEL_cter = 0;

	MEL_Camera default_camera = {0, 0, 0};
	/* Main loop */
	while (!glfwWindowShouldClose(win.window)){
		glfwPollEvents();
		glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
		glClear(GL_COLOR_BUFFER_BIT);
		smiley.rect.src[2] = smiley.rect.size[0];
		smiley.rect.src[3] = smiley.rect.size[1];

		/* Drawing */
		MEL_update_rect(win, Rend, r, default_camera, MEL_IMAGE_STATIC);
		MEL_update_image(win, Rend, crate,  camera, MEL_IMAGE_STATIC);
		MEL_update_image(win, Rend, smiley, camera, MEL_IMAGE_DYNAMIC);
		//r.color[3] += 0.001;

		/* * * * * */
		MEL_currt = glfwGetTime();
		MEL_tdiff = MEL_currt - MEL_prevt;
		++MEL_cter;
		if (MEL_tdiff >= (1.0f / 30.0f)){
			double MEL_fps = (1.0f/MEL_tdiff) * MEL_cter;
			MEL_prevt = MEL_currt;
			MEL_cter = 0;
			//printf("%ffps\n", MEL_fps);
		}

		glfwSwapBuffers(win.window);
	}

		/* * * * * * */

		/* Terminating */
		MEL_destroy_image(smiley);
		MEL_destroy_image(crate);
		MEL_Renderer2D_destroy(Rend);
		glfwDestroyWindow(win.window);
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
						glfwSetWindowShouldClose(window, GLFW_TRUE);
						break;
					case GLFW_KEY_F11:
						/* Fullscreen the game */
						toggleFullscreen(window, win.mode);
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
						camera[1] -= 10;
						//smiley.rect.pos[1] -= 10;
						break;
					case GLFW_KEY_A:
						camera[0] -= 10;
						//smiley.rect.pos[0] -= 10;
						break;
					case GLFW_KEY_S:
						camera[1] += 10;
						//smiley.rect.pos[1] += 10;
						break;
					case GLFW_KEY_D:
						camera[0] += 10;
						//smiley.rect.pos[0] += 10;
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
