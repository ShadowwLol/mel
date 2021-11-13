#ifndef _MEL_RECT_H
#define _MEL_RECT_H

#include "MEL_opengl.h"
#include "MEL_render.h"

typedef struct{
	vec2 pos;
	vec2 size;
	vec4 color;
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 mvp;
	GLfloat rotation;
	GLfloat vertices[32];
	GLuint indices[6];
} MEL_Rect;

#define MEL_update_rect(MELW, Renderer, Rect, Camera, Config){\
	glBindVertexArray(Renderer.rect_items.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.rect_items.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.rect_items.EBO);\
	if (((Rect.pos[0] > MELW.mode->width) || ((Rect.pos[0]+Rect.size[0]) < 0)) ||\
	((Rect.pos[1] > MELW.mode->height) || ((Rect.pos[1]+Rect.size[1]) < 0))){\
		{\
			Rect = rect_update_rect(Rect);\
		}\
	}else{\
		{\
			Rect = rect_update_rect(Rect);\
   			glBufferData(GL_ARRAY_BUFFER, sizeof(Rect.vertices), Rect.vertices, Config);\
			vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
			vec3 pivot = {(float)(Rect.pos[0] + Rect.size[0]/2.0f), (float)(Rect.pos[1] + Rect.size[1]/2.0f), 0.0f};\
			glm_ortho(0.0f, (float)MELW.mode->width, (float)MELW.mode->height, 0.0f, -1.0f, 1.0f, Rect.projection);\
			vec3 v = {Camera[0]*-1, Camera[1]*-1, Camera[2]*-1};\
			glm_mat4_identity(Rect.view);\
			glm_translate(Rect.view, v);\
			glm_mat4_identity(Rect.model);\
			glm_rotate_at(Rect.model, pivot, glm_rad(Rect.rotation), rotation_axis);\
			glm_mat4_mul(Rect.projection, Rect.view, Rect.mvp);\
			glm_mat4_mul(Rect.mvp, Rect.model, Rect.mvp);\
		}\
		glUseProgram(Renderer.rect_items.shader);\
		glUniformMatrix4fv(glGetUniformLocation(Renderer.rect_items.shader, "mvp"), 1, GL_FALSE, (const GLfloat *)Rect.mvp);\
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
		glUseProgram(0);\
		glBindTexture(GL_TEXTURE_2D, 0);\
	}\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

MEL_Rect MEL_load_rect(MEL_Renderer2D);
MEL_Rect rect_update_rect(MEL_Rect);

#endif
