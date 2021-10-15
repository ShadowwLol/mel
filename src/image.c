#include "../include/MEL_logs.h"
#include "../include/MEL_image.h"
#include <cglm/affine.h>
#include <cglm/util.h>

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLuint width, GLuint height, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#else
Image image_load_image(GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLuint width, GLuint height, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#endif
	stbi_set_flip_vertically_on_load(true);
	Image img = {
		.data = stbi_load(path, (int *)&img.rect.w, (int *)&img.rect.h, &img.channels, 0),
		.rect.x = x,
		.rect.y = y,
		.rect.color[0] = R,
		.rect.color[1] = G,
		.rect.color[2] = B,
		.rect.rotation = rotation,
		.rect.vertices = {
		    // positions                        // colors // texture coords
		    x+img.rect.h, y,              0.0f,  R, G, B,  1.0f, 1.0f, // top right
		    x+img.rect.h, y-img.rect.h,   0.0f,  R, G, B,  1.0f, 0.0f, // bottom right
		    x,            y-img.rect.h,   0.0f,  R, G, B,  0.0f, 0.0f, // bottom left
		    x,            y,              0.0f,  R, G, B,  0.0f, 1.0f  // top left <-- anchor point
		},
		.indices = {
		    0, 1, 3, // first triangle
		    1, 2, 3  // second triangle
		},
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
	    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.rect.w, img.rect.h, 0, channels, GL_UNSIGNED_BYTE, img.data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, 0);
	    log_log(LOG_SUCCESS, "Successfully loaded image : {%s}", path);
	}else{
	    log_log(LOG_ERROR, "Failed loading image : {%s}", path);
	}
	stbi_image_free(img.data);
	return img;
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
		.w = source.rect.w,
		.h = source.rect.h,
		.color[0] = source.rect.color[0],
		.color[1] = source.rect.color[1],
		.color[2] = source.rect.color[2],
		.rotation = source.rect.rotation,
    	.vertices = {
    	    // positions                                                     // colors                                                          // texture coords
    	    source.rect.x,               source.rect.y+source.rect.h, 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  0.0f, 0.0f, // bottom left
    	    source.rect.x,               source.rect.y,               0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  0.0f, 1.0f,  // top left <-- anchor point
    	    source.rect.x+source.rect.w, source.rect.y,               0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  1.0f, 1.0f, // top right
    	    source.rect.x+source.rect.w, source.rect.y+source.rect.h, 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  1.0f, 0.0f, // bottom right
		}
	};
	return img;
}
