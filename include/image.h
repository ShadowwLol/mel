#ifndef IMAGE_H
#define IMAGE_H

#include "../include/opengl.h"
#include "../include/shader.h"
#include "../include/def.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint bound;
    GLchar * path;
    GLint width;
    GLint height;
    GLint channels;
    GLuchar * data;
    GLuint texture;
	Shader image_shader;

	Rect rect;
	GLuint indices[6];
} Image;

#define logical_width(size_provided) ((float)size_provided/(float)window.mode->width)
#define logical_height(size_provided) ((float)size_provided/(float)window.mode->height)
#define bind_image_texture(type, image){if (!image.bound){glBindTexture(type, image.texture); image.bound = 1;}}
#define MEL_prepare_image(out){\
    glGenVertexArrays(1, &out.VAO);\
    glGenBuffers(1, &out.VBO);\
    glGenBuffers(1, &out.EBO);\
    glBindVertexArray(out.VAO);\
    glBindBuffer(GL_ARRAY_BUFFER, out.VBO);\
    glBufferData(GL_ARRAY_BUFFER, sizeof(out.rect.vertices), out.rect.vertices, GL_STATIC_DRAW);\
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.EBO);\
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(out.indices), out.indices, GL_STATIC_DRAW);\
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
#define MEL_update_image(image){\
    glBindVertexArray(image.VAO);\
    glBindBuffer(GL_ARRAY_BUFFER, image.VBO);\
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image.EBO);\
	{\
		image.rect = image_update_image(image);\
   		glBufferData(GL_ARRAY_BUFFER, sizeof(image.rect.vertices), image.rect.vertices, GL_STATIC_DRAW);\
		vec3 rotation_axis = {0.0f, 0.0f, 1.0f};\
		vec3 pivot = {(float)(image.rect.x + image.width/2.0f), (float)(image.rect.y + image.height/2.0f), 0.0f};\
		glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, image.rect.projection);\
		glm_rotate_at(image.rect.projection, pivot, glm_rad(image.rect.rotation), rotation_axis);\
	}\
	glBindTexture(GL_TEXTURE_2D, image.texture);\
	glUseProgram(image.image_shader.shader_program);\
	glUniform1i(glGetUniformLocation(image.image_shader.shader_program, "texture1"), 0);\
	glUniformMatrix4fv(glGetUniformLocation(image.image_shader.shader_program, "projection"), 1, GL_FALSE, (const GLfloat *)image.rect.projection);\
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
	glDeleteVertexArrays(1, &image.VAO);\
	glDeleteBuffers(1, &image.VBO);\
	glDeleteBuffers(1, &image.EBO);\
	glDeleteProgram(image.image_shader.shader_program);\
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
Rect image_update_image(Image source);

#endif
