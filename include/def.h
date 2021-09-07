#ifndef DEF_H
#define DEF_H

#include "../include/opengl.h"
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)
#define WINDOW_TITLE "Game"

typedef struct{
    GLFWwindow * window;
    const GLFWvidmode* mode;
} Window;

typedef struct {
    const GLchar * vertex_shader;
    const GLchar * fragment_shader;
    GLuint shader_program;
} Shader;

typedef struct {
    GLint x;
    GLint y;
} ivec2;

typedef struct {
    unsigned long int w;
    unsigned long int h;
} Size;

#endif