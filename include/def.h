#ifndef DEF_H
#define DEF_H

#include "../include/opengl.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>

#define WINDOW_WIDTH (1280.0f)
#define WINDOW_HEIGHT (720.0f)
#define WINDOW_TITLE "Game"

#define ASPECT_RATIO_W (16.0f)
#define ASPECT_RATIO_H (9.0f)
#define ASPECT_RATIO (ASPECT_RATIO_W/ASPECT_RATIO_H)

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
	mat4 projection;
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
    uint32_t w;
    uint32_t h;
} Size;

#endif
