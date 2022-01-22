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

#define MEL_FN unsigned char
#define EX_S (0)
#define EX_F (255)

#define MEL_KEY unsigned char

#define MEL_SINGLETON(t, inst, init) t* MEL_S_##t_##inst() { static t inst = init; return &inst; }

#define MEL_set_clear_color(r, g, b, a) glClearColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
#define MEL_clear(buffer) glClear(buffer);

typedef struct{
	GLFWwindow * window;
	GLFWvidmode* mode;
} MEL_Window;

typedef struct{
	MEL_Window window_ctx;  /* Window context                                     */
	String title;           /* Window title                                       */
	double_t width, height; /* Window dimensions       => [widthxheight]          */
	bool vsync;         /* Vertical Syncronization => Prevents screen tearing */
	GLint aa_samples;       /* Antialiasing samples < MEL_get_max_aa_samples()    */
} MEL_ctx;

#endif
