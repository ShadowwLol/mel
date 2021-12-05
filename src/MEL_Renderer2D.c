#include "../include/MEL_def.h"
#include "../include/MEL_Renderer2D.h"
#include "../include/MEL_Texture.h"
#include "../include/MEL_Rect.h"

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window win){
	GLint MT;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MT);\
	MEL_Renderer2D Renderer = {
		/* Image */
		.image_items.tex_count = 0,
		.image_items.MAX_TEXTURES = MT,
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
	glm_ortho(0.0f, (float)win.mode->width, (float)win.mode->height, 0.0f, -1.0f, 1.0f, Renderer.projection);
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
