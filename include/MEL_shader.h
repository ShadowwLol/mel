#ifndef SHADER_H
#define SHADER_H

#include "../include/MEL_def.h"

GLuint MEL_create_shader_program(const char * vert_path, const char * frag_path);
GLchar * MEL_read_from_file(const GLchar * path);

#endif
