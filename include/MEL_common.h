#ifndef _MEL_COMMON_H
#define _MEL_COMMON_H

#define PRINT_DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);

#include "MEL_opengl.h"
#include "MEL_IO.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#if __WIN32
#include <windef.h>
#include <winbase.h>
#include <wincon.h>
#endif

//#define WINDOW_WIDTH (1280.0f)
//#define WINDOW_HEIGHT (720.0f)
//#define WINDOW_TITLE "Game"

typedef char MEL_bool;
#define MEL_TRUE (1)
#define MEL_FALSE (0)

#define MEL_SINGLETON(t, inst, init) t* MEL_S_##t_##inst() { static t inst = init; return &inst; }

typedef struct{
	GLFWwindow * window;
	GLFWvidmode* mode;
} MEL_Window;

typedef struct{
	MEL_Window window_ctx;  /* Window context                                     */
	String title;           /* Window title                                       */
	double_t width, height; /* Window dimensions       => [widthxheight]          */
	MEL_bool vsync;         /* Vertical Syncronization => Prevents screen tearing */
	GLint aa_samples;       /* Antialiasing samples < MEL_get_max_aa_samples()    */
} MEL_ctx;

#endif
