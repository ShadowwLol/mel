#include "../include/MEL_opengl.h"
#include "../include/MEL_def.h"
#include <stdio.h>

int toggleFullscreen(GLFWwindow * window, GLFWvidmode * win_mode){
    if (!glfwGetWindowMonitor(window)){
        /* Currently windowed */
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), (((float)win_mode->width/2) - (WINDOW_WIDTH/2)), (((float)win_mode->height/2) - (WINDOW_HEIGHT/2)), WINDOW_WIDTH, WINDOW_HEIGHT, win_mode->refreshRate);
    }else{
        /* Currently fullscreen */
        glfwSetWindowMonitor(window, NULL, (((float)win_mode->width/2) - (WINDOW_WIDTH/2)), (((float)win_mode->height/2) - (WINDOW_HEIGHT/2)), WINDOW_WIDTH, WINDOW_HEIGHT, win_mode->refreshRate);
    }
    return 0;
}
