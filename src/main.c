#include "../include/MEL_opengl.h" /* for gl defs */
#include "../include/MEL_IO.h"     /* for i/o     */
#include "../include/MEL_def.h"    /* for configs */
#include "../include/MEL_shader.h" /* for shaders */
#include "../include/MEL_logs.h"   /* for logging */
#include "../include/MEL_image.h"  /* for images  */
#include "../include/MEL_rect.h"   /* for rects   */
#include "../include/MEL_misc.h"   /* for misc    */

MEL_Window win;
Image smiley;
MEL_Camera camera;

#if __WIN32
HANDLE hConsole;
WORD saved_attributes;

int main(void){
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
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

	/* OPENGL configuration  */
	gladLoadGL();
	glfwSwapInterval(0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	/* * * * * * * * * * * * */

	/* Setting callbacks  */
	glfwSetErrorCallback(error_callback);
	glfwSetJoystickCallback(joystick_callback);
	glfwSetKeyCallback(win.window, key_callback);
	glfwSetWindowSizeCallback(win.window, window_size_callback);
	glfwSetWindowSizeLimits(win.window, (win.mode->width/2), (win.mode->height/2), win.mode->width, win.mode->height);
	glfwSetScrollCallback(win.window, scroll_callback);
	/* * * * * * * * * * */

	/* Images  */
	MEL_Renderer2D Rend = MEL_Renderer2D_init();
	smiley = MEL_load_image(Rend, "resources/images/smiley.png", GL_RGBA, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	smiley.rect.pos[0] = 640.0f;
	smiley.rect.pos[1] = 360.0f;
	smiley.rect.color[3] = 0.5f;
	Image crate = MEL_load_image(Rend, "resources/images/container.jpg", GL_RGB, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
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
		MEL_calculate_delta();
		MEL_calculate_FPS();
		glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
		glClear(GL_COLOR_BUFFER_BIT);

		/* Drawing */
		MEL_update_rect(win, Rend, r, default_camera, MEL_IMAGE_STATIC);
		MEL_update_image(win, Rend, crate,  camera, MEL_IMAGE_STATIC);
		MEL_update_image(win, Rend, smiley, camera, MEL_IMAGE_DYNAMIC);
		/* * * * * */

		printf("%d\n", MEL_fps());
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
