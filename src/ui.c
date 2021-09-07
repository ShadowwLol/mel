#include "../include/opengl.h"
#include "../include/def.h"
#include <stdio.h>

int toggleFullscreen(Window window){
    if (!glfwGetWindowMonitor(window.window)){
        /* Currently windowed */
        glfwSetWindowMonitor(window.window, glfwGetPrimaryMonitor(), ((window.mode->width/2) - (WINDOW_WIDTH/2)), ((window.mode->height/2) - (WINDOW_HEIGHT/2)), WINDOW_WIDTH, WINDOW_HEIGHT, window.mode->refreshRate);
    }else{
        /* Currently fullscreen */
        glfwSetWindowMonitor(window.window, NULL, ((window.mode->width/2) - (WINDOW_WIDTH/2)), ((window.mode->height/2) - (WINDOW_HEIGHT/2)), WINDOW_WIDTH, WINDOW_HEIGHT, window.mode->refreshRate);
    }
    return 0;
}