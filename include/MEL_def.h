#ifndef DEF_H
#define DEF_H

#define PRINT_DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);

#include "../include/MEL_opengl.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>

#define WINDOW_WIDTH (1280.0f)
#define WINDOW_HEIGHT (720.0f)
#define WINDOW_TITLE "Game"

#define ASPECT_RATIO_W (16.0f)
#define ASPECT_RATIO_H (9.0f)
#define ASPECT_RATIO (ASPECT_RATIO_W/ASPECT_RATIO_H)

typedef uint8_t MEL_bool;
#define MEL_TRUE (1)
#define MEL_FALSE (0)

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
