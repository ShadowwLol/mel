#ifndef MEL_RENDER_H
#define MEL_RENDER_H

#include "MEL_opengl.h"

#define IMAGE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define IMAGE_FRAG_SHADER_PATH "resources/shaders/image.frag"
#define RECT_VERT_SHADER_PATH  "resources/shaders/basic.vert"
#define RECT_FRAG_SHADER_PATH  "resources/shaders/basic.frag"

//#define MAX_QUADS (20000)
//#define MAX_VERTICES (MAX_QUADS*4)
//#define MAX_INDICES (MAX_QUADS*6)

typedef struct {
	mat4 projection;
	GLuint VAO, VBO, EBO;
    GLuint img_shader;
	GLuint rect_shader;
	GLint tex_count, MAX_TEXTURES;
} MEL_Renderer2D;

#define MEL_Renderer2D_init(Renderer){\
	Renderer.tex_count = 0;\
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Renderer.MAX_TEXTURES);\
	glGenVertexArrays(1, &Renderer.VAO);\
	glGenBuffers(1, &Renderer.VBO);\
	glGenBuffers(1, &Renderer.EBO);\
	Renderer.img_shader = shader_create_shader_program(IMAGE_VERT_SHADER_PATH, IMAGE_FRAG_SHADER_PATH);\
	Renderer.rect_shader = shader_create_shader_program(RECT_VERT_SHADER_PATH, RECT_FRAG_SHADER_PATH);\
}

#define MEL_Renderer2D_destroy(Renderer){\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
	glDeleteVertexArrays(1, &Renderer.VAO);\
	glDeleteBuffers(1, &Renderer.VBO);\
	glDeleteBuffers(1, &Renderer.EBO);\
	glDeleteProgram(Renderer.img_shader);\
	glDeleteProgram(Renderer.rect_shader);\
}

#endif
