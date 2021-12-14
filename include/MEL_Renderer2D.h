#ifndef _MEL_RENDER_H
#define _MEL_RENDER_H

#include "MEL_opengl.h"
#include "MEL_def.h"

#define IMAGE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define IMAGE_FRAG_SHADER_PATH "resources/shaders/image.frag"
#define RECT_VERT_SHADER_PATH  "resources/shaders/basic.vert"
#define RECT_FRAG_SHADER_PATH  "resources/shaders/basic.frag"

typedef struct{
	GLuint VAO, VBO, EBO, shader;
	GLint tex_count, MAX_TEXTURES;
	GLuint indices[6];
} MEL_Renderer2D_image_items;

typedef struct{
	GLuint VAO, VBO, EBO, shader;
	GLuint indices[6];
} MEL_Renderer2D_rect_items;

typedef struct {
	MEL_Renderer2D_image_items image_items;
	MEL_Renderer2D_rect_items rect_items;
	mat4 projection;
} MEL_Renderer2D;

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

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window);

#endif