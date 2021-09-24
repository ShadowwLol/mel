#ifndef SHADER_H
#define SHADER_H

#include "../include/def.h"

#if __WIN32
#include <windows.h>
GLuint create_shader_program(HANDLE hConsole, WORD saved_attributes, Shader shader);
#define shader_create_shader_program(shader) create_shader_program(hConsole, saved_attributes, shader)
#else
GLuint create_shader_program(Shader shader);
#define shader_create_shader_program(shader) create_shader_program(shader)
#endif
GLchar * read_from_file(const GLchar * path);

#endif
