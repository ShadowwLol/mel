#ifndef _MEL_DEF_H
#define _MEL_DEF_H

#define PRINT_DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);

#include "MEL_opengl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#if __WIN32
#include <windef.h>
#include <winbase.h>
#include <wincon.h>
extern HANDLE hConsole;
extern WORD saved_attributes;
#endif

#define WINDOW_WIDTH (1280.0f)
#define WINDOW_HEIGHT (720.0f)
#define WINDOW_TITLE "Game"

typedef uint8_t MEL_bool;
#define MEL_TRUE (1)
#define MEL_FALSE (0)

#define MEL_SINGLETON(t, inst, init) t* MEL_S_##t_##inst() { static t inst = init; return &inst; }

typedef struct{
	GLFWwindow * window;
	GLFWvidmode* mode;
} MEL_Window;

void key_callback(GLFWwindow *, int, int, int, int);
void scroll_callback(GLFWwindow *, double, double);
void window_size_callback(GLFWwindow *, int, int);
void error_callback(int, const char *);
void joystick_callback(int, int);
int toggleFullscreen(MEL_Window);

#endif
