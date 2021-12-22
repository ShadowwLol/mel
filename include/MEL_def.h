#ifndef _MEL_DEF_H
#define _MEL_DEF_H

#include "MEL_logs.h"
#include "MEL_IO.h"

//static char _title[1024];
static String _title;
static double_t _width, _height;
static MEL_bool _vsync;

static char * MEL_get_error_description();
static int32_t MEL_get_error_id();

static MEL_ctx MEL_ctx_init(const char * title, double_t width, double_t height, MEL_bool vsync);
static void update_window(MEL_ctx ctx);
static void MEL_quit(MEL_ctx * ctx);

void input(GLFWwindow *, int, int, int, int);
void window_size_callback(GLFWwindow *, int, int);
void error_callback(int, const char *);
void joystick_callback(int, int);
int MEL_toggle_fullscreen(MEL_ctx ctx);

inline static MEL_ctx MEL_ctx_init(const char * title, double_t width, double_t height, MEL_bool vsync){
	//MEL_strlcpy(_title, title, MEL_strlen(title));
	//_title = init_str()
	_width = width;
	_height = height;
	_vsync = vsync;

	MEL_ctx ctx = {
		.width = width,
		.height = height,
		.vsync = vsync
	};

	ctx.title = init_str();
	_title = init_str();
	set_str(&ctx.title, title);
	set_str(&_title, ctx.title.buffer);

	/* Initializing the library  */
	if (!glfwInit()){
		MEL_log(LOG_ERROR, "Failed initializing GLFW");
		return ctx;
	}else{
		MEL_log(LOG_SUCCESS, "Successfully initialized GLFW");
	}
	/* * * * * * * * * * * * * * */

	/* Window hints  */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, 1);
	/* * * * * * * * * * * */

	/* Creating the window */
	ctx.window_ctx.mode = (GLFWvidmode *)glfwGetVideoMode(glfwGetPrimaryMonitor());
	ctx.window_ctx.window = glfwCreateWindow(ctx.width, ctx.height, ctx.title.buffer, glfwGetPrimaryMonitor(), NULL);
	if (!ctx.window_ctx.window){
		MEL_log(LOG_ERROR, "Failed creating window");
		glfwTerminate();
		return ctx;
	}else{
		MEL_log(LOG_SUCCESS, "Successfully created window");
	}
	/* * * * * * * * * * * */

	/* Make the window's context current */
	glfwMakeContextCurrent(ctx.window_ctx.window);
	/* * * * * * * * * * * * * * * * * * */

	/* OPENGL configuration  */
	gladLoadGL();
	glfwSwapInterval(ctx.vsync);
	glViewport(0, 0, ctx.width, ctx.height);
	/* * * * * * * * * * * * */

	/* Callbacks */
	glfwSetErrorCallback(error_callback);
	glfwSetInputMode(ctx.window_ctx.window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetKeyCallback(ctx.window_ctx.window, input);
	glfwSetFramebufferSizeCallback(ctx.window_ctx.window, window_size_callback);
	glfwSetWindowSizeLimits(ctx.window_ctx.window, (ctx.window_ctx.mode->width/2), (ctx.window_ctx.mode->height/2), ctx.window_ctx.mode->width, ctx.window_ctx.mode->height);
	/* * * * * * */

	return (ctx);
}

inline static void update_window(MEL_ctx ctx){
	if (MEL_strcmp(ctx.title.buffer, _title.buffer) != 0){
		set_str(&_title, ctx.title.buffer);
		glfwSetWindowTitle(ctx.window_ctx.window, _title.buffer);
	}

	if (ctx.width != _width || ctx.height != _height){
		_width = ctx.width;
		_height = ctx.height;
		glfwSetWindowSize(ctx.window_ctx.window, _width, _height);
		window_size_callback(ctx.window_ctx.window, _width, _height);
	}

	if (ctx.vsync != _vsync){
		_vsync = ctx.vsync;
		MEL_log(LOG_INFORMATION, "Vsync: [%d]", _vsync);
		glfwSwapInterval(_vsync);
	}
}

inline static void MEL_quit(MEL_ctx * ctx){
	remove_str(&ctx->title);
	remove_str(&_title);
	glfwDestroyWindow(ctx->window_ctx.window);
	glfwTerminate();
}

#endif
