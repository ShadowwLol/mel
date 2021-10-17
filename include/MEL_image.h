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
	vec3 color;           /* Image color modulation [r,g,b]   */
	GLfloat rotation;     /* Image rotation deg               */
	GLfloat vertices[32]; /* Image vertices                   */
} rect;

typedef struct {
    stbi_uc * data;
	GLuint id;
	GLint type;
    GLuint texture;
	struct rect rect;
	GLuint indices[6];
} Image;

#define MEL_update_image(Renderer, Img, Config){\
	glBindVertexArray(Renderer.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
	if (((Img.rect.pos[0] > WINDOW_WIDTH) || ((Img.rect.pos[0]+Img.rect.size[0]) < 0)) ||\
	((Img.rect.pos[1] > WINDOW_HEIGHT) || ((Img.rect.pos[1]+Img.rect.size[1]) < 0))){\
		{\
			Img.rect = image_update_image(Img);\
		}\
	}else{\
		glBindVertexArray(Renderer.VAO);\
		glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
		{\
			Img.rect = image_update_image(Img);\
   			glBufferData(GL_ARRAY_BUFFER, sizeof(Img.rect.vertices), Img.rect.vertices, Config);\
			vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
			vec3 pivot = {(float)(Img.rect.pos[0] + Img.rect.size[0]/2.0f), (float)(Img.rect.pos[1] + Img.rect.size[1]/2.0f), 0.0f};\
			glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, Renderer.projection);\
			glm_rotate_at(Renderer.projection, pivot, glm_rad(Img.rect.rotation), rotation_axis);\
		}\
		glBindTexture(GL_TEXTURE_2D, Img.texture);\
		glUseProgram(Renderer.img_shader);\
		glUniform1i(glGetUniformLocation(Renderer.img_shader, "texture1"), Img.id);\
		glUniformMatrix4fv(glGetUniformLocation(Renderer.img_shader, "projection"), 1, GL_FALSE, (const GLfloat *)Renderer.projection);\
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
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

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, MEL_Renderer2D Renderer, GLchar * path, GLenum channels);
#define img_load_image(Rend, path, channels) image_load_image(hConsole, saved_attributes, Rend, path, channels);
#else
Image image_load_image(MEL_Renderer2D Renderer, GLchar * path, GLenum channels);
#define img_load_image(Rend, path, channels) image_load_image(Rend, path, channels);
#endif

struct rect image_update_image(Image source);

#endif
