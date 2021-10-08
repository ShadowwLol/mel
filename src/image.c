#include "../include/MEL_logs.h"
#include "../include/MEL_image.h"
#include <cglm/affine.h>
#include <cglm/util.h>

#define IMAGE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define IMAGE_FRAG_SHADER_PATH "resources/shaders/image.frag"

int8_t MEL_image_cmp(Image i, Image j){
	/* Position*/
	if (i.rect.x == j.rect.x && i.rect.y == j.rect.y &&\
	/* Color*/\
	i.rect.R == j.rect.R && i.rect.G == j.rect.G && i.rect.B == i.rect.B &&\
	/* Rotation */\
	i.rect.rotation == j.rect.rotation){
		return 0;
	}else{
		return 1;
	}
}

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLuint width, GLuint height, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#else
Image image_load_image(GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLuint width, GLuint height, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#endif
	stbi_set_flip_vertically_on_load(true);
	if (!width || !height){
    	Image img = {
			.changed = MEL_TRUE,
			.path = path,
    		.data = stbi_load(img.path, &img.width, &img.height, &img.channels, 0),
			.rect.x = x,
			.rect.y = y,
			.rect.R = R,
			.rect.G = G,
			.rect.B = B,
			.rect.rotation = rotation,
    		.rect.vertices = {
    		    // positions                        // colors // texture coords
    		    x+img.width, y,              0.0f,  R, G, B,  1.0f, 1.0f, // top right
    		    x+img.width, y-img.height,   0.0f,  R, G, B,  1.0f, 0.0f, // bottom right
    		    x,           y-img.height,   0.0f,  R, G, B,  0.0f, 0.0f, // bottom left
    		    x,           y,              0.0f,  R, G, B,  0.0f, 1.0f  // top left <-- anchor point
			},
			.indices = {
    		    0, 1, 3, // first triangle
    		    1, 2, 3  // second triangle
    		},
			.image_shader.vertex_shader = IMAGE_VERT_SHADER_PATH,
			.image_shader.fragment_shader = IMAGE_FRAG_SHADER_PATH,
			.image_shader.shader_program = shader_create_shader_program(img.image_shader)
		};
    	glGenTextures(1, &img.texture);
    	glBindTexture(GL_TEXTURE_2D, img.texture);

    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    	GLfloat border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    	if (img.data){
    	    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.width, img.height, 0, channels, GL_UNSIGNED_BYTE, img.data);
    	    glGenerateMipmap(GL_TEXTURE_2D);
    	    glBindTexture(GL_TEXTURE_2D, 0);
    	    log_log(LOG_SUCCESS, "Successfully loaded image : {%s}", img.path);
    	}else{
    	    log_log(LOG_ERROR, "Failed loading image : {%s}", img.path);
    	}
    	stbi_image_free(img.data);
    	return img;
	}else{
    	Image img = {
			.path = path,
    		.data = stbi_load(img.path, &img.width, &img.height, &img.channels, 0),
			.rect.x = x,
			.rect.y = y,
			.rect.R = R,
			.rect.G = G,
			.rect.B = B,
			.rect.rotation = rotation,
    		.rect.vertices = {
    		    // positions                        // colors // texture coords
    		    x+img.width, y,              0.0f,  R, G, B,  1.0f, 1.0f, // top right
    		    x+img.width, y-img.height,   0.0f,  R, G, B,  1.0f, 0.0f, // bottom right
    		    x,           y-img.height,   0.0f,  R, G, B,  0.0f, 0.0f, // bottom left
    		    x,           y,              0.0f,  R, G, B,  0.0f, 1.0f  // top left <-- anchor point
			},
			.indices = {
    		    0, 1, 3, // first triangle
    		    1, 2, 3  // second triangle
    		},
			.image_shader.vertex_shader = IMAGE_VERT_SHADER_PATH,
			.image_shader.fragment_shader = IMAGE_FRAG_SHADER_PATH,
			.image_shader.shader_program = shader_create_shader_program(img.image_shader)
		};
    	glGenTextures(1, &img.texture);
    	glBindTexture(GL_TEXTURE_2D, img.texture);

    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    	GLfloat border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    	if (img.data){
    	    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.width, img.height, 0, channels, GL_UNSIGNED_BYTE, img.data);
    	    glGenerateMipmap(GL_TEXTURE_2D);
    	    glBindTexture(GL_TEXTURE_2D, 0);
    	    log_log(LOG_SUCCESS, "Successfully loaded image : {%s}", img.path);
    	}else{
    	    log_log(LOG_ERROR, "Failed loading image : {%s}", img.path);
    	}
    	stbi_image_free(img.data);
		glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, img.rect.projection);
    	return img;
	}
}

/*
void image_draw_image(Image img, mat4 projection){
	if (img.VAO == 0){
    	glGenVertexArrays(1, &img.VAO);
    	glBindVertexArray(img.VAO);
	}
	if (img.VBO == 0){
    	glGenBuffers(1, &img.VBO);
    	glBindBuffer(GL_ARRAY_BUFFER, img.VBO);
	}

    glBufferData(GL_ARRAY_BUFFER, sizeof(img.vertices), img.vertices, GL_STATIC_DRAW);

	if (img.EBO == 0){
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, img.EBO);
	}

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(img.indices), img.indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
	DEBUG;
	bind_image_texture(GL_TEXTURE_2D, img);
	DEBUG;
	glUseProgram(img.image_shader.shader_program);
	DEBUG;
	glUniform1i(glGetUniformLocation(img.image_shader.shader_program, "texture1"), 0);
	DEBUG;
	glUniformMatrix4fv(glGetUniformLocation(img.image_shader.shader_program, "projection"), 1, GL_FALSE, (const GLfloat *)projection);
	DEBUG;
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	DEBUG;
}
*/

struct rect image_update_image(Image source){
	struct rect img = {
		.x = source.rect.x,
		.y = source.rect.y,
		.w = source.width,
		.h = source.height,
		.R = source.rect.R,
		.G = source.rect.G,
		.B = source.rect.B,
		.rotation = source.rect.rotation,
    	.vertices = {
    	    // positions                                                    // colors                                     // texture coords
    	    source.rect.x,              source.rect.y+source.height, 0.0f,  source.rect.R, source.rect.G, source.rect.B,  0.0f, 0.0f, // bottom left
    	    source.rect.x,              source.rect.y,               0.0f,  source.rect.R, source.rect.G, source.rect.B,  0.0f, 1.0f,  // top left <-- anchor point
    	    source.rect.x+source.width, source.rect.y,               0.0f,  source.rect.R, source.rect.G, source.rect.B,  1.0f, 1.0f, // top right
    	    source.rect.x+source.width, source.rect.y+source.height, 0.0f,  source.rect.R, source.rect.G, source.rect.B,  1.0f, 0.0f, // bottom right
		}
	};
	return img;
}
