#ifndef _MEL_IMAGE_H
#define _MEL_IMAGE_H

#include "MEL_opengl.h"
#include "MEL_Renderer2D.h"
#include "MEL_shader.h"
#include "MEL_def.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

typedef struct {
	GLuint id;      /* Texture ID: [i]   */
	GLuint texture; /* Texture   : [i]   */
	MEL_Rect rect;  /* Rect      : [...] */
} MEL_Texture;

#define MEL_draw_tex(MELW, Renderer, Img, Camera){\
	if (((Img.rect.pos[0] > MELW.mode->width) || ((Img.rect.pos[0]+Img.rect.size[0]) < 0)) ||\
	((Img.rect.pos[1] > MELW.mode->height) || ((Img.rect.pos[1]+Img.rect.size[1]) < 0))){\
		{\
			MEL_update_tex(&Renderer, Img);\
		}\
	}else{\
		glBindVertexArray(Renderer.VAO);\
		glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
		{\
			MEL_update_tex(&Renderer, Img);\
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Renderer.geometry.vertices), Renderer.geometry.vertices);\
			glm_mat4_identity(Img.rect.model);\
			glm_rotate_at(Img.rect.model, (vec3){(float)(Img.rect.pos[0] + Img.rect.size[0]/2.0f), (float)(Img.rect.pos[1] + Img.rect.size[1]/2.0f), 0.0f}, glm_rad(Img.rect.rotation), (vec3){0.0f, 0.0f, 1.0f});\
			glm_mat4_mul(Renderer.projection, Camera.view, Img.rect.mvp);\
			glm_mat4_mul(Img.rect.mvp, Img.rect.model, Img.rect.mvp);\
		}\
		/* https://stackoverflow.com/a/28360579/16946028 */\
		/* glActiveTexture(GL_TEXTURE0+Img.id); <= Only when batching */\
		glBindTexture(GL_TEXTURE_2D, Img.texture);\
		glUseProgram(Renderer.shader);\
		glDrawElements(GL_TRIANGLES, (sizeof(Renderer.geometry.vertices)/sizeof(Renderer.geometry.vertices[0])), GL_UNSIGNED_INT, 0);\
	}\
}

#define MEL_destroy_image(image){\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glDeleteTextures(1, &image.texture);\
}

MEL_Texture MEL_load_tex(MEL_Renderer2D *, GLchar *, GLenum, GLenum, GLenum);
void MEL_update_tex(MEL_Renderer2D * Renderer, MEL_Texture source);

#endif
