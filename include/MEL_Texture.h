#ifndef _MEL_IMAGE_H
#define _MEL_IMAGE_H

#include "MEL_opengl.h"
#include "MEL_Renderer2D.h"
#include "MEL_shader.h"
#include "MEL_def.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

typedef struct {
	GLuint texture; /* Texture   : [i]   */
	MEL_Rect rect;  /* Rect      : [...] */
} MEL_Texture;

#define MEL_destroy_image(image){\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glDeleteTextures(1, &image.texture);\
}

MEL_Texture MEL_load_tex(MEL_Renderer2D *, GLchar *, GLenum, GLenum, GLenum);
void MEL_draw_tex(MEL_Window MELW, MEL_Renderer2D * Renderer, MEL_Texture * Img, MEL_Camera Camera);

#endif
