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
    GLint x;
    GLint y;
	GLuint w;
	GLuint h;
	GLfloat R;
	GLfloat G;
	GLfloat B;
	GLfloat rotation;
    vec3 color_mod;
    GLfloat vertices[32];
} Rect;

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
