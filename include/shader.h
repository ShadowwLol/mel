#ifndef SHADER_H
#define SHADER_H

#include "../include/def.h"

GLuint create_shader_program(Shader shader);
GLchar * read_from_file(const GLchar * path);

#endif