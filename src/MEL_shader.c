#include "../include/MEL_shader.h"
#include "../include/MEL_common.h"

GLuint MEL_create_shader_program(const char * vert_path, const char * frag_path){
	GLint success = 0;
	GLchar errnoo[1025] = "\0";
	String vertex_source = MEL_read_file(vert_path);
	String fragment_source = MEL_read_file(frag_path);

	if (!vertex_source.length || !fragment_source.length){
		MEL_log(LOG_ERROR, MEL_get_error_description());
		return -1;
	}

    /* Create vertex shader object and its reference */
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    /* Attach vertex shader source to the vertex shader object */
    glShaderSource(vertex_shader, 1, (const GLchar * const *)&vertex_source.buffer, NULL);
    /* Compile the vertex shader */
    glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE){
		glGetShaderInfoLog(vertex_shader, 1024, NULL, errnoo);
		MEL_log(LOG_ERROR, "Failed compiling vertex shader : {%s}\n[%s]", vert_path, errnoo);
		glDeleteShader(vertex_shader);
		return -1;
	}else{
		MEL_log(LOG_SUCCESS, "Successfully compiled vertex shader : {%s}", vert_path);
	}

    /* Create vertex shader object and its reference */
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    /* Attach vertex shader source to the vertex shader object */
    glShaderSource(fragment_shader, 1, (const GLchar * const *)&fragment_source.buffer, NULL);
    /* Compile the vertex shader */
    glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE){
		glGetShaderInfoLog(fragment_shader, 1024, NULL, errnoo);
		MEL_log(LOG_ERROR, "Failed compiling fragment shader : {%s}\n[%s]", frag_path, errnoo);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return -1;
	}else{
		MEL_log(LOG_SUCCESS, "Successfully compiled fragment shader : {%s}", frag_path);
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
