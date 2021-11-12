#include "../include/MEL_opengl.h"
#include "../include/MEL_def.h"
#include "../include/MEL_shader.h"
#include "../include/MEL_logs.h"

GLchar * MEL_read_from_file(const GLchar * path){
	FILE *f = fopen(path, "rt");
	if (!f){return (void *)-1;}
	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buffer = (char *) malloc(length + 1);
	buffer[length] = '\0';
	fread(buffer, 1, length, f);
	fclose(f);
	return buffer;
}

GLuint MEL_create_shader_program(const char * vert_path, const char * frag_path){
	const GLchar * vertex_source = MEL_read_from_file(vert_path);
    const GLchar * fragment_source = MEL_read_from_file(frag_path);

	if ((void *)-1 == vertex_source || (void *)-1 == fragment_source){return -1;}

    /* Create vertex shader object and its reference */
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    /* Attach vertex shader source to the vertex shader object */
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    /* Compile the vertex shader */
    glCompileShader(vertex_shader);

	GLint success = 0;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE){
		log_log(LOG_ERROR, "Failed compiling vertex shader {%s}", vert_path);
		glDeleteShader(vertex_shader);
		return -1;
	}else{
		log_log(LOG_SUCCESS, "Successfully compiled vertex shader {%s}", vert_path);
	}

    /* Create vertex shader object and its reference */
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    /* Attach vertex shader source to the vertex shader object */
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    /* Compile the vertex shader */
    glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE){
		log_log(LOG_ERROR, "Failed compiling fragment shader {%s}", frag_path);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return -1;
	}else{
		log_log(LOG_SUCCESS, "Successfully compiled fragment shader {%s}", frag_path);
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
    free((void*)vertex_source);
    free((void*)fragment_source);

    return shader_program;
}
