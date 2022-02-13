#include "../include/MEL_opengl.h"
#include "../include/MEL_common.h"

void MEL_toggle_fullscreen(MEL_ctx ctx){
	if (!glfwGetWindowMonitor(ctx.window_ctx.window)){
		/* Currently windowed */
		glfwSetWindowMonitor(ctx.window_ctx.window, glfwGetPrimaryMonitor(), (((float)ctx.window_ctx.mode->width/2.0f) - (ctx.width/2.0f)), (((float)ctx.window_ctx.mode->height/2.0f) - (ctx.height/2.0f)), ctx.width, ctx.height, ctx.window_ctx.mode->refreshRate);
	}else{
		/* Currently fullscreen */
		glfwSetWindowMonitor(ctx.window_ctx.window, NULL, (((float)ctx.window_ctx.mode->width/2.0f) - (ctx.height/2.0f)), (((float)ctx.window_ctx.mode->height/2.0f) - (ctx.height/2.0f)), ctx.width, ctx.height, ctx.window_ctx.mode->refreshRate);
	}
}

void MEL_local_mouse_pos(MEL_ctx ctx, double * x, double * y){
	glfwGetCursorPos(ctx.window_ctx.window, x, y);

#if 0
	if (glfwGetWindowMonitor(ctx.window_ctx.window)){
		/* Fullscreen */
		return;
	}
#endif

	vec2 scale = { (ctx.window_ctx.n_width/ctx.width), (ctx.window_ctx.n_height/ctx.height) };
	vec2 real = { ctx.window_ctx.n_width * scale[1], ctx.window_ctx.n_height * scale[0]};
    vec2 r = {((ctx.window_ctx.n_width/2) - (real[0]/2)), ((ctx.window_ctx.n_height/2) - (real[1]/2))};
    vec2 adjustedMouse = {*x - r[0], *y - r[1]};

    *x = (float) adjustedMouse[0] / (float)real[0] * (float)ctx.width;
    *y = (float) adjustedMouse[1] / (float)real[1] * (float)ctx.height;

}
