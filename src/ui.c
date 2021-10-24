#include "../include/MEL_opengl.h"
#include "../include/MEL_def.h"
#include <stdio.h>

int toggleFullscreen(MEL_Window window){
    if (!glfwGetWindowMonitor(window.window)){
        /* Currently windowed */
        glfwSetWindowMonitor(window.window, glfwGetPrimaryMonitor(), (((float)window.mode->width/2) - (WINDOW_WIDTH/2)), (((float)window.mode->height/2) - (WINDOW_HEIGHT/2)), WINDOW_WIDTH, WINDOW_HEIGHT, window.mode->refreshRate);
    }else{
        /* Currently fullscreen */
        glfwSetWindowMonitor(window.window, NULL, (((float)window.mode->width/2) - (WINDOW_WIDTH/2)), (((float)window.mode->height/2) - (WINDOW_HEIGHT/2)), WINDOW_WIDTH, WINDOW_HEIGHT, window.mode->refreshRate);
    }
    return 0;
}
