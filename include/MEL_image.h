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
} MEL_Renderer2D;

static struct rect{
	GLint x, y;
	GLuint w, h;
	GLfloat R, G, B;
	GLfloat rotation;
   	vec3 color_mod;
	mat4 projection;
   	GLfloat vertices[32];
} rect;

typedef struct {
    GLchar * path;
    GLint width;
    GLint height;
    GLint channels;
    GLuchar * data;
    GLuint texture;
	struct rect rect;
	GLuint indices[6];
} Image;

#define MEL_Renderer2D_init(Renderer){\
	glGenVertexArrays(1, &Renderer.VAO);\
	glGenBuffers(1, &Renderer.VBO);\
	glGenBuffers(1, &Renderer.EBO);\
	Renderer.shader_program = shader_create_shader_program(IMAGE_VERT_SHADER_PATH, IMAGE_FRAG_SHADER_PATH);\
}

#define logical_width(size_provided) ((float)size_provided/(float)window.mode->width)
#define logical_height(size_provided) ((float)size_provided/(float)window.mode->height)
#define MEL_prepare_image(Renderer, Img){\
    glBindVertexArray(Renderer.VAO);\
    glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
    glBufferData(GL_ARRAY_BUFFER, sizeof(Img.rect.vertices), Img.rect.vertices, GL_STATIC_DRAW);\
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Img.indices), Img.indices, GL_STATIC_DRAW);\
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);\
    glEnableVertexAttribArray(0);\
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));\
    glEnableVertexAttribArray(1);\
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));\
    glEnableVertexAttribArray(2);\
    glBindVertexArray(0);\
    glBindBuffer(GL_ARRAY_BUFFER, 0);\
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

#define MEL_update_image(Renderer, Img){\
	glBindVertexArray(Renderer.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
	{\
		Img.rect = image_update_image(Img);\
   		glBufferData(GL_ARRAY_BUFFER, sizeof(Img.rect.vertices), Img.rect.vertices, GL_STATIC_DRAW);\
		vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
		vec3 pivot = {(float)(Img.rect.x + Img.width/2.0f), (float)(Img.rect.y + Img.height/2.0f), 0.0f};\
		glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, Img.rect.projection);\
		glm_rotate_at(Img.rect.projection, pivot, glm_rad(Img.rect.rotation), rotation_axis);\
	}\
	glBindTexture(GL_TEXTURE_2D, Img.texture);\
	glUseProgram(Renderer.shader_program);\
	glUniform1i(glGetUniformLocation(Renderer.shader_program, "texture1"), 0);\
	glUniformMatrix4fv(glGetUniformLocation(Renderer.shader_program, "projection"), 1, GL_FALSE, (const GLfloat *)Img.rect.projection);\
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
}

#define MEL_delete_image(image){\
	glBindTexture(GL_TEXTURE_2D, 0);\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\
	glDeleteTextures(1, &image.texture);\
}

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLuint width, GLuint height, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation);
#define img_load_image(path, channels, x, y, w, h, R, G, B, rotation) image_load_image(hConsole, saved_attributes, path, channels, x, y, w, h, R, G, B, rotation);
#else
Image image_load_image(GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLuint width, GLuint height, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation);
#define img_load_image(path, channels, x, y, w, h, R, G, B, rotation) image_load_image(path, channels, x, y, w, h, R, G, B, rotation);
#endif

//void image_init(GLuint * VAO, GLuint * VBO, GLuint * EBO, vec2 pos, vec2 size, GLfloat R, GLfloat G, GLfloat B);
//void image_draw_image(Image img, mat4 projection);
int8_t MEL_image_cmp(Image i, Image j);
struct rect image_update_image(Image source);

#endif
