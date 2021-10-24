#include "../include/MEL_render.h"
#include "../include/MEL_image.h"
#include "../include/MEL_rect.h"

#if __WIN32
#include <windows.h>
extern HANDLE hConsole;
extern WORD saved_attributes;
#endif

MEL_Renderer2D MEL_Renderer2D_init(){
	MEL_Renderer2D tmp;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tmp.image_items.MAX_TEXTURES);\
	MEL_Renderer2D Renderer = {
		/* Image */
		.image_items.tex_count = 0,
		.image_items.MAX_TEXTURES = tmp.image_items.MAX_TEXTURES,
		.image_items.shader = MEL_create_shader_program(IMAGE_VERT_SHADER_PATH, IMAGE_FRAG_SHADER_PATH),
		.image_items.indices = {
			0, 1, 3,
			1, 2, 3
		},
		/* Rectangle */
		.rect_items.shader = MEL_create_shader_program(RECT_VERT_SHADER_PATH, RECT_FRAG_SHADER_PATH),
		.rect_items.indices = {
			0, 1, 3,
			1, 2, 3
		},
	};
	/* Image */
	glGenVertexArrays(1, &Renderer.image_items.VAO);
	glGenBuffers(1, &Renderer.image_items.VBO);
	glGenBuffers(1, &Renderer.image_items.EBO);
	/* Rectangle */
	glGenVertexArrays(1, &Renderer.rect_items.VAO);
	glGenBuffers(1, &Renderer.rect_items.VBO);
	glGenBuffers(1, &Renderer.rect_items.EBO);
	return Renderer;
}
