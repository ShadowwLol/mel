#ifndef _MEL_IMAGE_H
#define _MEL_IMAGE_H

#include "MEL_opengl.h"
#include "MEL_Renderer2D.h"
#include "MEL_shader.h"
#include "MEL_def.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#define MEL_IMAGE_STATIC  GL_STATIC_DRAW
#define MEL_IMAGE_DYNAMIC GL_DYNAMIC_DRAW

typedef struct rect{
	vec2 pos;             /* Image position         [x,y]     */
	vec2 size;            /* Image size             [w,h]     */
	vec4 color;           /* Image color modulation [r,g,b]   */
	GLfloat rotation;     /* Image rotation deg               */
	GLfloat vertices[104]; /* Image vertices                   */
} rect;

typedef struct {
	mat4 model;
	mat4 mvp;
	GLuint id;
	GLuint texture;
	rect rect;
} MEL_Texture;

#define MEL_update_image(MELW, Renderer, Img, Camera, Config){\
	glBindVertexArray(Renderer.image_items.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.image_items.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.image_items.EBO);\
	if (((Img.rect.pos[0] > MELW.mode->width) || ((Img.rect.pos[0]+Img.rect.size[0]) < 0)) ||\
	((Img.rect.pos[1] > MELW.mode->height) || ((Img.rect.pos[1]+Img.rect.size[1]) < 0))){\
		{\
			Img.rect = image_update_image(Img);\
		}\
	}else{\
		glBindVertexArray(Renderer.image_items.VAO);\
		glBindBuffer(GL_ARRAY_BUFFER, Renderer.image_items.VBO);\
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.image_items.EBO);\
		{\
			Img.rect = image_update_image(Img);\
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Img.rect.vertices), Img.rect.vertices);\
			glm_mat4_identity(Img.model);\
			glm_rotate_at(Img.model, (vec3){(float)(Img.rect.pos[0] + Img.rect.size[0]/2.0f), (float)(Img.rect.pos[1] + Img.rect.size[1]/2.0f), 0.0f}, glm_rad(Img.rect.rotation), (vec3){0.0f, 0.0f, 1.0f});\
			glm_mat4_mul(Renderer.projection, Camera.view, Img.mvp);\
			glm_mat4_mul(Img.mvp, Img.model, Img.mvp);\
		}\
		/* https://stackoverflow.com/a/28360579/16946028 */\
		/* glActiveTexture(GL_TEXTURE0+Img.id); <= Only when batching */\
		glBindTexture(GL_TEXTURE_2D, Img.texture);\
		glUseProgram(Renderer.image_items.shader);\
		glDrawElements(GL_TRIANGLES, (sizeof(Img.rect.vertices)/sizeof(Img.rect.vertices[0])), GL_UNSIGNED_INT, 0);\
		glUseProgram(0);\
		glBindTexture(GL_TEXTURE_2D, 0);\
	}\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

#define MEL_destroy_image(image){\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glDeleteTextures(1, &image.texture);\
}

MEL_Texture MEL_load_image(MEL_Renderer2D *, GLchar *, GLenum, GLenum, GLenum);
rect image_update_image(MEL_Texture);

#endif
