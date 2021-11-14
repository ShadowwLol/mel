#include "../include/MEL_def.h"
#include "../include/MEL_logs.h"
#include "../include/MEL_image.h"
#include <cglm/affine.h>
#include <cglm/util.h>

Image MEL_load_image(MEL_Renderer2D Renderer, GLchar * path, GLenum channels, GLenum min_filter, GLenum max_filter){
	stbi_set_flip_vertically_on_load(true);
	GLint w, h, c;
	Image img = {
		.data = stbi_load(path, &w, &h, &c, 0),
		.mvp = GLM_MAT4_IDENTITY_INIT,
		.projection = GLM_MAT4_IDENTITY_INIT,
		.view = GLM_MAT4_IDENTITY_INIT,
		.model = GLM_MAT4_IDENTITY_INIT,
		.rect.pos[0] = 0.0f,
		.rect.pos[1] = 0.0f,
		.rect.size[0] = w,
		.rect.size[1] = h,
		.rect.color[0] = 1.0f,
		.rect.color[1] = 1.0f,
		.rect.color[2] = 1.0f,
		.rect.color[3] = 1.0f,
		.rect.rotation = 0.0f,
		.rect.vertices = {
		    // positions                                                               colors                                                                      tex coords    sampler
		    img.rect.pos[0],                  img.rect.pos[1],                  0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 1.0f, 1.0f,   img.id, // top right
		    img.rect.pos[0],                  img.rect.pos[1]+img.rect.size[1], 0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 1.0f, 0.0f,   img.id, // bottom right
		    img.rect.pos[0]+img.rect.size[0], img.rect.pos[1]+img.rect.size[1], 0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 0.0f, 0.0f,   img.id, // bottom left
		    img.rect.pos[0]+img.rect.size[0], img.rect.pos[1],                  0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 0.0f, 1.0f,   img.id, // top left <-- anchor point
		}
	};
	if (Renderer.image_items.tex_count < Renderer.image_items.MAX_TEXTURES){
		++Renderer.image_items.tex_count;
	}else{
		Renderer.image_items.tex_count = 1;
	}
	glActiveTexture(GL_TEXTURE0+(Renderer.image_items.tex_count-1));
	glGenTextures(1, &img.texture);
	glBindTexture(GL_TEXTURE_2D, img.texture);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	if (img.data){
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.rect.size[0], img.rect.size[1], 0, channels, GL_UNSIGNED_BYTE, img.data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	    log_log(LOG_SUCCESS, "Successfully loaded image : {%s}", path);
	}else{
	    log_log(LOG_ERROR, "Failed loading image : {%s}", path);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img.data);

	img.id = Renderer.image_items.tex_count-1;

	glBindVertexArray(Renderer.image_items.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.image_items.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(img.rect.vertices), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.image_items.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Renderer.image_items.indices), Renderer.image_items.indices, GL_STATIC_DRAW);

	/* Position Attribute [x,y,z] */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/* Color Attribute [R,G,B,A] */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/* Texture Coords Attribute [x, y] */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	/* Sampler Attribute [i] */
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return img;
}

struct rect image_update_image(Image source){
	struct rect img = {
		.pos[0] = source.rect.pos[0],
		.pos[1] = source.rect.pos[1],
		.size[0] = source.rect.size[0],
		.size[1] = source.rect.size[1],
		.color[0] = source.rect.color[0],
		.color[1] = source.rect.color[1],
		.color[2] = source.rect.color[2],
		.color[3] = source.rect.color[3],
		.rotation = source.rect.rotation,
    	.vertices = {
    	    // positions                                                                           colors                                                                                  tex coords  sampler
    	    source.rect.pos[0],                     source.rect.pos[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 0.0f, 0.0f, source.id, // bottom left
    	    source.rect.pos[0],                     source.rect.pos[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 0.0f, 1.0f, source.id, // top left <-- anchor point
    	    source.rect.pos[0]+source.rect.size[0], source.rect.pos[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 1.0f, 1.0f, source.id, // top right
    	    source.rect.pos[0]+source.rect.size[0], source.rect.pos[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 1.0f, 0.0f, source.id, // bottom right
		}
	};
	return img;
}
