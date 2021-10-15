#ifndef IMAGE_H
#define IMAGE_H

#include "../include/MEL_opengl.h"
#include "../include/MEL_shader.h"
#include "../include/MEL_def.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define IMAGE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define IMAGE_FRAG_SHADER_PATH "resources/shaders/image.frag"

typedef struct {
	GLuint VAO, VBO, EBO;
    GLuint shader_program;
	GLuint tex_count, MAX_TEXTURES;
} MEL_Renderer2D;

static struct rect{
	vec2 coord;
	vec2 size;
	vec3 color;
	GLfloat rotation;
	mat4 projection;
   	GLfloat vertices[32];
} rect;

typedef struct {
    stbi_uc * data;
	GLuint id;
    GLuint texture;
	struct rect rect;
	GLuint indices[6];
} Image;

#define MEL_Renderer2D_init(Renderer){\
	Renderer.tex_count = 0;\
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Renderer.MAX_TEXTURES);\
	glGenVertexArrays(1, &Renderer.VAO);\
	glGenBuffers(1, &Renderer.VBO);\
	glGenBuffers(1, &Renderer.EBO);\
	Renderer.shader_program = shader_create_shader_program(IMAGE_VERT_SHADER_PATH, IMAGE_FRAG_SHADER_PATH);\
}

#define MEL_Renderer2D_destroy(Renderer){\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
	glDeleteVertexArrays(1, &Renderer.VAO);\
	glDeleteBuffers(1, &Renderer.VBO);\
	glDeleteBuffers(1, &Renderer.EBO);\
	glDeleteProgram(Renderer.shader_program);\
}

#define MEL_update_image(Renderer, Img){\
	glBindVertexArray(Renderer.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
	{\
		Img.rect = image_update_image(Img);\
   		glBufferData(GL_ARRAY_BUFFER, sizeof(Img.rect.vertices), Img.rect.vertices, GL_DYNAMIC_DRAW);\
		vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
		vec3 pivot = {(float)(Img.rect.coord[0] + Img.rect.size[0]/2.0f), (float)(Img.rect.coord[1] + Img.rect.size[1]/2.0f), 0.0f};\
		glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, Img.rect.projection);\
		glm_rotate_at(Img.rect.projection, pivot, glm_rad(Img.rect.rotation), rotation_axis);\
	}\
	glBindTexture(GL_TEXTURE_2D, Img.texture);\
	glUseProgram(Renderer.shader_program);\
	glUniform1i(glGetUniformLocation(Renderer.shader_program, "texture1"), Img.id);\
	glUniformMatrix4fv(glGetUniformLocation(Renderer.shader_program, "projection"), 1, GL_FALSE, (const GLfloat *)Img.rect.projection);\
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

#define MEL_destroy_image(image){\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glDeleteTextures(1, &image.texture);\
}

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, MEL_Renderer2D Renderer, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation);
#define img_load_image(Rend, path, channels, x, y, R, G, B, rotation) image_load_image(hConsole, saved_attributes, Rend, path, channels, x, y, R, G, B, rotation);
#else
Image image_load_image(MEL_Renderer2D Renderer, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation);
#define img_load_image(Rend, path, channels, x, y, R, G, B, rotation) image_load_image(Rend, path, channels, x, y, R, G, B, rotation);
#endif

struct rect image_update_image(Image source);

#endif
