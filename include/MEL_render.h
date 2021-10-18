#ifndef MEL_RENDER_H
#define MEL_RENDER_H

#include "MEL_opengl.h"

#define IMAGE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define IMAGE_FRAG_SHADER_PATH "resources/shaders/image.frag"
#define RECT_VERT_SHADER_PATH  "resources/shaders/basic.vert"
#define RECT_FRAG_SHADER_PATH  "resources/shaders/basic.frag"

typedef struct{
	GLuint VAO, VBO, EBO, shader;
	GLint tex_count, MAX_TEXTURES;
} MEL_Renderer2D_image_items;

typedef struct{
	GLuint VAO, VBO, EBO, shader;
} MEL_Renderer2D_rect_items;

typedef struct {
	mat4 projection;
	MEL_Renderer2D_image_items image_items;
	MEL_Renderer2D_rect_items rect_items;
} MEL_Renderer2D;

#define MEL_Renderer2D_init(Renderer){\
	Renderer.image_items.tex_count = 0;\
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Renderer.image_items.MAX_TEXTURES);\
	glGenVertexArrays(1, &Renderer.image_items.VAO);\
	glGenBuffers(1, &Renderer.image_items.VBO);\
	glGenBuffers(1, &Renderer.image_items.EBO);\
	Renderer.image_items.shader = shader_create_shader_program(IMAGE_VERT_SHADER_PATH, IMAGE_FRAG_SHADER_PATH);\
	glGenVertexArrays(1, &Renderer.rect_items.VAO);\
	glGenBuffers(1, &Renderer.rect_items.VBO);\
	glGenBuffers(1, &Renderer.rect_items.EBO);\
	Renderer.rect_items.shader = shader_create_shader_program(RECT_VERT_SHADER_PATH, RECT_FRAG_SHADER_PATH);\
}

#define MEL_Renderer2D_destroy(Renderer){\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
	glDeleteVertexArrays(1, &Renderer.image_items.VAO);\
	glDeleteBuffers(1, &Renderer.image_items.VBO);\
	glDeleteBuffers(1, &Renderer.image_items.EBO);\
	glDeleteProgram(Renderer.image_items.shader);\
	glDeleteVertexArrays(1, &Renderer.rect_items.VAO);\
	glDeleteBuffers(1, &Renderer.rect_items.VBO);\
	glDeleteBuffers(1, &Renderer.rect_items.EBO);\
	glDeleteProgram(Renderer.rect_items.shader);\
}

#endif
