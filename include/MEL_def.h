#ifndef _MEL_DEF_H
#define _MEL_DEF_H

#include "MEL_common.h"
#include "MEL_logs.h"
#include "MEL_IO.h"
#include "MEL_misc.h"

static String _title;
static double_t _width, _height;
static bool _vsync;
static GLint _aa_samples;
static GLint _max_aa_samples;

static GLint MEL_get_max_aa_samples();
static MEL_ctx MEL_ctx_init(const char * title, double_t width, double_t height, bool vsync);
static void update_window(MEL_ctx * ctx);
static void MEL_quit(MEL_ctx * ctx);

static void MEL_poll_events(MEL_ctx * ctx);

/* Input */
static int16_t keyboard[512];

#define MEL_KEY_NONE    (0)
#define MEL_KEY_PRESS   (1)
#define MEL_KEY_REPEAT  (2)
#define MEL_KEY_RELEASE (3)

static void k_input(GLFWwindow *, int, int, int, int);
inline static void k_input(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_RELEASE){
		action = MEL_KEY_RELEASE;
	}
	keyboard[key] = action;
}

static int8_t MEL_get_key(int16_t key);
inline static int8_t MEL_get_key(int16_t key){return keyboard[key];}

void input(void);
void MEL_render(void);
/* * * * */
void window_size_callback(GLFWwindow *, int, int);
void error_callback(int, const char *);
void joystick_callback(int, int);
void MEL_toggle_fullscreen(MEL_ctx ctx);
void MEL_global_mouse_pos(MEL_ctx ctx, double * x, double * y);
void MEL_local_mouse_pos(MEL_ctx ctx, double * x, double * y);

inline static GLint MEL_get_max_aa_samples(){return _max_aa_samples;}

inline static MEL_ctx MEL_ctx_init(const char * title, double_t width, double_t height, bool vsync){
	_width = width;
	_height = height;
	_vsync = vsync;

	MEL_ctx ctx = {
		.width = width,
		.height = height,
		.vsync = vsync,
		.aa_samples = 0
	};

	ctx.title = init_str();
	_title = init_str();
	set_str(&ctx.title, title);
	set_str(&_title, ctx.title.buffer);

	/* Initializing the library  */
	if (!glfwInit()){
		MEL_log(LOG_ERROR, "Failed initializing GLFW");
		exit(EX_F);
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
        exit(EX_F);
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

	glGetIntegerv(GL_MAX_SAMPLES, &_max_aa_samples);
	_aa_samples = _max_aa_samples;
	ctx.aa_samples = _aa_samples;
	glfwWindowHint(GLFW_SAMPLES, _aa_samples);
	/* * * * * * * * * * * * */

	/* Callbacks */
	glfwSetErrorCallback(error_callback);
	glfwSetInputMode(ctx.window_ctx.window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetKeyCallback(ctx.window_ctx.window, k_input);
	glfwSetFramebufferSizeCallback(ctx.window_ctx.window, window_size_callback);
	glfwSetWindowSizeLimits(ctx.window_ctx.window, (ctx.window_ctx.mode->width/2), (ctx.window_ctx.mode->height/2), ctx.window_ctx.mode->width, ctx.window_ctx.mode->height);
	/* * * * * * */

	/* Initializing keyboard */
	for (uint16_t i = 0; i < (sizeof(keyboard)/sizeof(keyboard[0])); ++i){
		keyboard[i] = MEL_KEY_NONE;
	}
	/* * * * * * * * * * * * */
	return (ctx);
}

inline static void update_window(MEL_ctx * ctx){
	if (strncmp(ctx->title.buffer, _title.buffer,
				(_title.length > ctx->title.length ? _title.length : ctx->title.length)) != 0){
		set_str(&_title, ctx->title.buffer);
		glfwSetWindowTitle(ctx->window_ctx.window, _title.buffer);
	}

	if (ctx->width != _width || ctx->height != _height){
		_width = ctx->width;
		_height = ctx->height;
		glfwSetWindowSize(ctx->window_ctx.window, _width, _height);
		window_size_callback(ctx->window_ctx.window, _width, _height);
	}

	if (ctx->vsync != _vsync){
		_vsync = ctx->vsync;
		glfwSwapInterval(_vsync);
	}

	if (ctx->aa_samples != _aa_samples){
		if (ctx->aa_samples <= _max_aa_samples && ctx->aa_samples > 0){
			/* Minimum => 1                         */
			/* Maximum => MEL_get_max_aa_samples(); */
			_aa_samples = ctx->aa_samples;
			glfwWindowHint(GLFW_SAMPLES, _aa_samples);
		}else{
			MEL_log(LOG_ERROR, "Invalid Antialiasing Samples [%d]. Restoring to [%d]", ctx->aa_samples, _aa_samples);
			ctx->aa_samples = _aa_samples;
		}
	}
}

inline static void MEL_quit(MEL_ctx * ctx){
	remove_str(&ctx->title);
	remove_str(&_title);
	glfwDestroyWindow(ctx->window_ctx.window);
	glfwTerminate();
}

inline static void MEL_poll_events(MEL_ctx * ctx){
	update_window(ctx);
	glfwPollEvents();
	input();
	for (uint16_t i = 0; i < (sizeof(keyboard)/sizeof(keyboard[0])); ++i){
		if (keyboard[i] == MEL_KEY_RELEASE){
			keyboard[i] = MEL_KEY_NONE;
		}
	}
	MEL_calculate_delta();
	MEL_calculate_fps();
}

#endif
