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
