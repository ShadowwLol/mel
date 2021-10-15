#include "../include/MEL_logs.h"
#include "../include/MEL_image.h"
#include <cglm/affine.h>
#include <cglm/util.h>

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, MEL_Renderer2D Renderer, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#else
Image image_load_image(MEL_Renderer2D Renderer, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#endif
	stbi_set_flip_vertically_on_load(true);
	GLint w, h, c;
	Image img = {
		.data = stbi_load(path, &w, &h, &c, 0),
		.rect.coord[0] = x,
		.rect.coord[1] = y,
		.rect.size[0] = w,
		.rect.size[1] = h,
		.rect.color[0] = R,
		.rect.color[1] = G,
		.rect.color[2] = B,
		.rect.rotation = rotation,
		.rect.vertices = {
		    // positions                                   // colors // texture coords
		    x+img.rect.size[0], y,                  0.0f,  R, G, B,  1.0f, 1.0f, // top right
		    x+img.rect.size[0], y+img.rect.size[1], 0.0f,  R, G, B,  1.0f, 0.0f, // bottom right
		    x,                  y+img.rect.size[1], 0.0f,  R, G, B,  0.0f, 0.0f, // bottom left
		    x,                  y,                  0.0f,  R, G, B,  0.0f, 1.0f  // top left <-- anchor point
		},
		.indices = {
		    0, 1, 3, // first triangle
		    1, 2, 3  // second triangle
		},
	};
	if (Renderer.tex_count < Renderer.MAX_TEXTURES){
		++Renderer.tex_count;
	}else{
		Renderer.tex_count = 1;
	}
	glActiveTexture(GL_TEXTURE0+(Renderer.tex_count-1));
	glGenTextures(1, &img.texture);
	glBindTexture(GL_TEXTURE_2D, img.texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	if (img.data){
	    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.rect.size[0], img.rect.size[1], 0, channels, GL_UNSIGNED_BYTE, img.data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, 0);
	    log_log(LOG_SUCCESS, "Successfully loaded image : {%s}", path);
	}else{
	    log_log(LOG_ERROR, "Failed loading image : {%s}", path);
	}
	stbi_image_free(img.data);

	img.id = Renderer.tex_count-1;\
	glBindVertexArray(Renderer.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
	glBufferData(GL_ARRAY_BUFFER, sizeof(img.rect.vertices), img.rect.vertices, GL_STATIC_DRAW);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(img.indices), img.indices, GL_STATIC_DRAW);\
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);\
	glEnableVertexAttribArray(0);\
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));\
	glEnableVertexAttribArray(1);\
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));\
	glEnableVertexAttribArray(2);\
	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\

	return img;
}

struct rect image_update_image(Image source){
	struct rect img = {
		.coord[0] = source.rect.coord[0],
		.coord[1] = source.rect.coord[1],
		.size[0] = source.rect.size[0],
		.size[1] = source.rect.size[1],
		.color[0] = source.rect.color[0],
		.color[1] = source.rect.color[1],
		.color[2] = source.rect.color[2],
		.rotation = source.rect.rotation,
    	.vertices = {
    	    // positions                                                                 // colors                                                          // texture coords
    	    source.rect.coord[0],                     source.rect.coord[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  0.0f, 0.0f, // bottom left
    	    source.rect.coord[0],                     source.rect.coord[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  0.0f, 1.0f,  // top left <-- anchor point
    	    source.rect.coord[0]+source.rect.size[0], source.rect.coord[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  1.0f, 1.0f, // top right
    	    source.rect.coord[0]+source.rect.size[0], source.rect.coord[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  1.0f, 0.0f, // bottom right
		}
	};
	return img;
}
