#ifndef _MEL_RENDER_H
#define _MEL_RENDER_H

#include "MEL_opengl.h"
#include "MEL_def.h"
#include "MEL_shader.h"

#define TEXTURE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define TEXTURE_FRAG_SHADER_PATH "resources/shaders/image.frag"

#define VERTEX_COUNT (104)
#define INDEX_COUNT (6)

#define MAX_QUAD_COUNT  (5)
#define MAX_VERTEX_COUNT (VERTEX_COUNT * MAX_QUAD_COUNT)
#define MAX_INDEX_COUNT  (INDEX_COUNT * MAX_QUAD_COUNT)

typedef struct{
	vec2 pos;
	vec2 size;
	vec4 color;
	mat4 model;
	mat4 mvp;
	GLfloat rotation;
} MEL_Rect;

typedef MEL_Rect MEL_ColorRect;

typedef struct {
	mat4 projection;
	GLuint * default_texture;

	GLuint VAO, VBO, EBO, shader;
	GLint ID, MAX_TEXTURES;
	GLuint indices[6];
	GLfloat vertices[104];
} MEL_Renderer2D;

#define MEL_Renderer2D_destroy(Renderer){\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
	glDeleteVertexArrays(1, &Renderer.VAO);\
	glDeleteBuffers(1, &Renderer.VBO);\
	glDeleteBuffers(1, &Renderer.EBO);\
	glDeleteProgram(Renderer.shader);\
}

#define MEL_begin_rendering2D(Renderer){\
	glBindVertexArray(Renderer.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
	glEnable(GL_BLEND);\
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);\
	glUseProgram(Renderer.shader);\
}

#define MEL_end_rendering2D(){\
	glUseProgram(0);\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
	glfwSwapBuffers(mctx.window_ctx.window);\
}

#include "MEL_Camera.h"

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window);
MEL_ColorRect MEL_init_rect(MEL_Renderer2D *);
void MEL_draw_rect(MEL_Window MELW, MEL_Renderer2D * Renderer, MEL_ColorRect * Rect, MEL_Camera Camera);

#endif
