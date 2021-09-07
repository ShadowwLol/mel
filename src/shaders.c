#include "../include/opengl.h"
#include "../include/def.h"
#include <stdio.h>
#include <stdlib.h>

GLchar * read_from_file(const GLchar * path){
    FILE * fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    GLuint size_of_file = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    GLchar * file_contents = malloc(size_of_file);
    if (file_contents){fread(file_contents, 1, size_of_file, fp);}
    fclose(fp);
    file_contents[size_of_file] = '\0';
    return file_contents;
}

GLuint create_shader_program(Shader shader){
    const GLchar * vertex_source = read_from_file(shader.vertex_shader);
    const GLchar * fragment_source = read_from_file(shader.fragment_shader);

    /* Create vertex shader object and its reference */
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    /* Attach vertex shader source to the vertex shader object */
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    /* Compile the vertex shader */
    glCompileShader(vertex_shader);

    /* Create vertex shader object and its reference */
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    /* Attach vertex shader source to the vertex shader object */
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    /* Compile the vertex shader */
    glCompileShader(fragment_shader);

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
    free((void*)vertex_source);
    free((void*)fragment_source);

    return shader_program;
}