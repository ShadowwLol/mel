#include "../include/MEL_logs.h"
#include "../include/MEL_image.h"
#include <cglm/affine.h>
#include <cglm/util.h>

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#else
Image image_load_image(GLchar * path, GLenum channels, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B, GLfloat rotation){
#endif
	stbi_set_flip_vertically_on_load(true);
	GLint w, h, c;
	Image img = {
		.data = stbi_load(path, &w, &h, &c, 0),
		.rect.x = x,
		.rect.y = y,
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
	return img;
}

struct rect image_update_image(Image source){
	struct rect img = {
		.x = source.rect.x,
		.y = source.rect.y,
		.size[0] = source.rect.size[0],
		.size[1] = source.rect.size[1],
		.color[0] = source.rect.color[0],
		.color[1] = source.rect.color[1],
		.color[2] = source.rect.color[2],
		.rotation = source.rect.rotation,
    	.vertices = {
    	    // positions                                                                 // colors                                                          // texture coords
    	    source.rect.x,                     source.rect.y+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  0.0f, 0.0f, // bottom left
    	    source.rect.x,                     source.rect.y,                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  0.0f, 1.0f,  // top left <-- anchor point
    	    source.rect.x+source.rect.size[0], source.rect.y,                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  1.0f, 1.0f, // top right
    	    source.rect.x+source.rect.size[0], source.rect.y+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2],  1.0f, 0.0f, // bottom right
		}
	};
	return img;
}
