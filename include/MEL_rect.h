#ifndef MEL_RECT_H
#define MEL_RECT_H

#include "MEL_opengl.h"
#include "MEL_render.h"

typedef struct{
	vec2 pos;
	vec2 size;
	vec4 color;
	GLfloat rotation;
	GLfloat vertices[32];
	GLuint indices[6];
} MEL_Rect;

#define MEL_update_rect(Renderer, Rect, Config){\
	glBindVertexArray(Renderer.rect_items.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.rect_items.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.rect_items.EBO);\
	if (((Rect.pos[0] > WINDOW_WIDTH) || ((Rect.pos[0]+Rect.size[0]) < 0)) ||\
	((Rect.pos[1] > WINDOW_HEIGHT) || ((Rect.pos[1]+Rect.size[1]) < 0))){\
		{\
			Rect = rect_update_rect(Rect);\
		}\
	}else{\
		{\
			Rect = rect_update_rect(Rect);\
   			glBufferData(GL_ARRAY_BUFFER, sizeof(Rect.vertices), Rect.vertices, Config);\
			vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
			vec3 pivot = {(float)(Rect.pos[0] + Rect.size[0]/2.0f), (float)(Rect.pos[1] + Rect.size[1]/2.0f), 0.0f};\
			glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, Renderer.projection);\
			glm_rotate_at(Renderer.projection, pivot, glm_rad(Rect.rotation), rotation_axis);\
		}\
		glUseProgram(Renderer.rect_items.shader);\
		glUniformMatrix4fv(glGetUniformLocation(Renderer.rect_items.shader, "projection"), 1, GL_FALSE, (const GLfloat *)Renderer.projection);\
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
		glUseProgram(0);\
		glBindTexture(GL_TEXTURE_2D, 0);\
	}\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

MEL_Rect rect_update_rect(MEL_Rect rect);

#if __WIN32
#include <windows.h>
MEL_Rect rect_load_rect(HANDLE hConsole, WORD saved_attributes, MEL_Renderer2D Renderer);
#define MEL_load_rect(Renderer) rect_load_rect(hConsole, saved_attributes, Renderer);
#else
MEL_Rect rect_load_rect(MEL_Renderer2D Renderer);
#define MEL_load_rect(Renderer) rect_load_rect(Renderer);
#endif

#endif
