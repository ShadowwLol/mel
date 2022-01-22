#ifndef _MEL_RENDER_H
#define _MEL_RENDER_H

#include "MEL_opengl.h"
#include "MEL_def.h"
#include "MEL_shader.h"

#define TEXTURE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define TEXTURE_FRAG_SHADER_PATH "resources/shaders/image.frag"

typedef struct{
	vec2 pos;
	vec2 size;
	vec4 color;
	mat4 model;
	mat4 mvp;
	GLfloat rotation;
	GLfloat vertices[104];
} MEL_Rect;

typedef MEL_Rect MEL_ColorRect;

typedef struct {
	mat4 projection;
	GLuint * default_texture;

	GLuint VAO, VBO, EBO, shader;
	GLint TEXTURE_COUNT, MAX_TEXTURES;
	GLuint indices[6];
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
}

#define MEL_end_rendering2D(){\
	glUseProgram(0);\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

#define MEL_draw_rect(MELW, Renderer, Rect, Camera){\
	if (((Rect.pos[0] > MELW.mode->width) || ((Rect.pos[0]+Rect.size[0]) < 0)) ||\
	((Rect.pos[1] > MELW.mode->height) || ((Rect.pos[1]+Rect.size[1]) < 0))){\
		{\
			Rect = MEL_update_rect(Rect);\
		}\
	}else{\
		glBindVertexArray(Renderer.VAO);\
		glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
		{\
			Rect = MEL_update_rect(Rect);\
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Rect.vertices), Rect.vertices);\
			glm_mat4_identity(Rect.model);\
			glm_rotate_at(Rect.model, (vec3){(float)(Rect.pos[0] + Rect.size[0]/2.0f), (float)(Rect.pos[1] + Rect.size[1]/2.0f), 0.0f}, glm_rad(Rect.rotation), (vec3){0.0f, 0.0f, 1.0f});\
			glm_mat4_mul(Renderer.projection, Camera.view, Rect.mvp);\
			glm_mat4_mul(Rect.mvp, Rect.model, Rect.mvp);\
		}\
		glBindTexture(GL_TEXTURE_2D, *Renderer.default_texture);\
		glUseProgram(Renderer.shader);\
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
	}\
}

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window);
MEL_ColorRect MEL_init_rect(MEL_Renderer2D *);
MEL_ColorRect MEL_update_rect(MEL_ColorRect);

#endif
