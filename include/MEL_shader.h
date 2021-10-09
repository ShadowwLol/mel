#ifndef SHADER_H
#define SHADER_H

#include "../include/MEL_def.h"

#if __WIN32
#include <windows.h>
GLuint create_shader_program(HANDLE hConsole, WORD saved_attributes, const char * vert_path, const char * frag_path);
#define shader_create_shader_program(v_path, f_path) create_shader_program(hConsole, saved_attributes, v_path, f_path)
#else
GLuint create_shader_program(const char * vert_path, const char * frag_path);
#define shader_create_shader_program(v_path, f_path) create_shader_program(v_path, f_path)
#endif
GLchar * read_from_file(const GLchar * path);

#endif
