#ifndef IMAGE_H
#define IMAGE_H

#include "../include/opengl.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

typedef struct {
    GLchar * path;
    GLint width;
    GLint height;
    GLint channels;
    GLuchar * data;
    GLuint texture;

    vec2 position;
    vec2 size;
    float rotation;
    vec3 color_mod;
    GLfloat vertices[];
} Image;


#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, GLchar * path);
#define img_load_image(path) image_load_image(hConsole, saved_attributes, path);
#else
Image image_load_image(GLchar * path);
#define img_load_image(path) image_load_image(path);
#endif

void image_init(GLuint VAO);
void draw_image(GLuint * VAO, GLuint * shader_program, GLuint * texture, vec2 position, vec2 size, float rotate, vec3 color);


#endif