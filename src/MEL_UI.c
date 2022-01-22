#include "../include/MEL_opengl.h"
#include "../include/MEL_common.h"

char MEL_toggle_fullscreen(MEL_ctx ctx){
    if (!glfwGetWindowMonitor(ctx.window_ctx.window)){
        /* Currently windowed */
        glfwSetWindowMonitor(ctx.window_ctx.window, glfwGetPrimaryMonitor(), (((float)ctx.window_ctx.mode->width/2.0f) - (ctx.width/2.0f)), (((float)ctx.window_ctx.mode->height/2.0f) - (ctx.height/2.0f)), ctx.width, ctx.height, ctx.window_ctx.mode->refreshRate);
    }else{
        /* Currently fullscreen */
        glfwSetWindowMonitor(ctx.window_ctx.window, NULL, (((float)ctx.window_ctx.mode->width/2.0f) - (ctx.height/2.0f)), (((float)ctx.window_ctx.mode->height/2.0f) - (ctx.height/2.0f)), ctx.width, ctx.height, ctx.window_ctx.mode->refreshRate);
    }
    return 0;
}
