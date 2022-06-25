#ifndef _MEL_SHADER_H
#define _MEL_SHADER_H

#include "mel.h"
#include "common.h"

static MEL_FN MEL_shader(const char* vert_path, const char* frag_path);
inline static MEL_FN MEL_shader(const char* vert_path, const char* frag_path) {
  GLint success = 0;
  GLchar errnoo[1025] = "\0";
  String vertex_source = init_str(MEL_read_file(vert_path));
  String fragment_source = init_str(MEL_read_file(frag_path));

  if (!vertex_source.length || !fragment_source.length) {
    mlog(LOG_WARNING, "Could not create shader program");
    return EX_F;
  }

  /* Create vertex shader object and its reference */
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  /* Attach vertex shader source to the vertex shader object */
  glShaderSource(vertex_shader, 1,
                 (const GLchar * const *) &vertex_source.buffer, NULL);
  /* Compile the vertex shader */
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertex_shader, 1024, NULL, errnoo);
    mlog(LOG_WARNING, "Failed compiling vertex shader : {%s}\n[%s]",
         vert_path, errnoo);
    glDeleteShader(vertex_shader);
    return EX_F;
  } else {
    mlog(LOG_SUCCESS, "Successfully compiled vertex shader : {%s}",
         vert_path);
  }

  /* Create vertex shader object and its reference */
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  /* Attach vertex shader source to the vertex shader object */
  glShaderSource(fragment_shader, 1,
                 (const GLchar * const *) &fragment_source.buffer, NULL);
  /* Compile the vertex shader */
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragment_shader, 1024, NULL, errnoo);
    mlog(LOG_WARNING, "Failed compiling fragment shader : {%s}\n[%s]",
         frag_path, errnoo);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return EX_F;
  } else {
    mlog(LOG_SUCCESS, "Successfully compiled fragment shader : {%s}",
         frag_path);
  }

  /* Create shader program object and its reference */
  GLuint shader_program = glCreateProgram();
  /* Attach the vertex and fragment shaders to the shader program */
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  /* Link the shaders together into a program */
  glLinkProgram(shader_program);

  /* Free vertex and fragment shader objects */
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  remove_str(&vertex_source);
  remove_str(&fragment_source);

  return shader_program;
}

static MEL_FN MEL_str2shader(const char* vert_path, const char* frag_path);
inline static MEL_FN MEL_str2shader(const char* vert_path,
                                    const char* frag_path) {
  GLint success = 0;
  GLchar errnoo[1025] = "\0";
  if (!strlen(vert_path) || !strlen(frag_path)) {
    mlog(LOG_WARNING, "Could not create shader program");
    return EX_F;
  }

  /* Create vertex shader object and its reference */
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  /* Attach vertex shader source to the vertex shader object */
  glShaderSource(vertex_shader, 1, (const GLchar * const *) &vert_path, NULL);
  /* Compile the vertex shader */
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertex_shader, 1024, NULL, errnoo);
    mlog(LOG_WARNING, "Failed compiling vertex shader: [%s]", errnoo);
    glDeleteShader(vertex_shader);
    return EX_F;
  } else {
    mlog(LOG_SUCCESS, "Successfully compiled vertex shader");
  }

  /* Create vertex shader object and its reference */
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  /* Attach vertex shader source to the vertex shader object */
  glShaderSource(fragment_shader, 1, (const GLchar * const *) &frag_path,
                 NULL);
  /* Compile the vertex shader */
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragment_shader, 1024, NULL, errnoo);
    mlog(LOG_WARNING, "Failed compiling fragment shader: [%s]", errnoo);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return EX_F;
  } else {
    mlog(LOG_SUCCESS, "Successfully compiled fragment shader");
  }

  /* Create shader program object and its reference */
  GLuint shader_program = glCreateProgram();
  /* Attach the vertex and fragment shaders to the shader program */
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  /* Link the shaders together into a program */
  glLinkProgram(shader_program);

  /* Free vertex and fragment shader objects */
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}

#endif
