#ifndef IMAGE_H
#define IMAGE_H

#include "MEL_opengl.h"
#include "MEL_render.h"
#include "MEL_shader.h"
#include "MEL_def.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MEL_IMAGE_STATIC  GL_STATIC_DRAW
#define MEL_IMAGE_DYNAMIC GL_DYNAMIC_DRAW
#define MEL_TYPE_IMAGE (0x00000001)
#define MEL_TYPE_SPRITESHEET (0x00000010)

static struct rect{
	vec2 pos;             /* Image position         [x,y]     */
	vec4 src;             /* Texture coordinates    [x,y,w,h] */
	vec2 size;            /* Image size             [w,h]     */
	vec4 color;           /* Image color modulation [r,g,b]   */
	GLfloat rotation;     /* Image rotation deg               */
	GLfloat vertices[36]; /* Image vertices                   */
} rect;

typedef struct {
    stbi_uc * data;
	GLuint id;
	GLint type;
    GLuint texture;
	struct rect rect;
	GLuint indices[6];
} Image;

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
   			glBufferData(GL_ARRAY_BUFFER, sizeof(Img.rect.vertices), Img.rect.vertices, Config);\
			vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
			vec3 pivot = {(float)(Img.rect.pos[0] + Img.rect.size[0]/2.0f), (float)(Img.rect.pos[1] + Img.rect.size[1]/2.0f), 0.0f};\
			glm_ortho(0.0f, (float)MELW.mode->width, (float)MELW.mode->height, 0.0f, -1.0f, 1.0f, Renderer.projection);\
			vec3 v = {Camera[0]*-1, Camera[1]*-1, Camera[2]*-1};\
			glm_translate(Renderer.projection, v);\
			glm_rotate_at(Renderer.projection, pivot, glm_rad(Img.rect.rotation), rotation_axis);\
		}\
		glBindTexture(GL_TEXTURE_2D, Img.texture);\
		glUseProgram(Renderer.image_items.shader);\
		glUniform1i(glGetUniformLocation(Renderer.image_items.shader, "texture1"), Img.id);\
		glUniformMatrix4fv(glGetUniformLocation(Renderer.image_items.shader, "projection"), 1, GL_FALSE, (const GLfloat *)Renderer.projection);\
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
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

Image MEL_load_image(MEL_Renderer2D Renderer, GLchar * path, GLenum channels);
struct rect image_update_image(Image source);

#endif
