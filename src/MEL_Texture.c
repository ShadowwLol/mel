#include "../include/MEL_def.h"
#include "../include/MEL_logs.h"
#include "../include/MEL_Texture.h"
#include <cglm/affine.h>
#include <cglm/util.h>

MEL_Texture MEL_load_image(MEL_Renderer2D * Renderer, GLchar * path, GLenum channels, GLenum min_filter, GLenum max_filter){
	if (Renderer->TEXTURE_COUNT < Renderer->MAX_TEXTURES){
		++Renderer->TEXTURE_COUNT;
	}else{
		Renderer->TEXTURE_COUNT = 2;
	}
	stbi_set_flip_vertically_on_load(true);
	GLint w, h, c;
	stbi_uc * data = stbi_load(path, &w, &h, &c, 0);
	MEL_Texture img = {
		.rect.mvp = GLM_MAT4_IDENTITY_INIT,
		.rect.model = GLM_MAT4_IDENTITY_INIT,
		.id = Renderer->TEXTURE_COUNT-1,
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
		    // positions                                                               colors                                                                      tex coords    sampler   mvp **                                                  **  **                                                      **  **                                                       ** **                                                       **
		    img.rect.pos[0],                  img.rect.pos[1],                  0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 1.0f, 1.0f,   img.id,   img.rect.mvp[0][0], img.rect.mvp[0][1], img.rect.mvp[0][2], img.rect.mvp[0][3], img.rect.mvp[1][0], img.rect.mvp[1][1], img.rect.mvp[1][2], img.rect.mvp[1][3], img.rect.mvp[2][0], img.rect.mvp[2][1], img.rect.mvp[2][2], img.rect.mvp[2][3], img.rect.mvp[3][0], img.rect.mvp[3][1], img.rect.mvp[3][2], img.rect.mvp[3][3], // top right
		    img.rect.pos[0],                  img.rect.pos[1]+img.rect.size[1], 0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 1.0f, 0.0f,   img.id,   img.rect.mvp[0][0], img.rect.mvp[0][1], img.rect.mvp[0][2], img.rect.mvp[0][3], img.rect.mvp[1][0], img.rect.mvp[1][1], img.rect.mvp[1][2], img.rect.mvp[1][3], img.rect.mvp[2][0], img.rect.mvp[2][1], img.rect.mvp[2][2], img.rect.mvp[2][3], img.rect.mvp[3][0], img.rect.mvp[3][1], img.rect.mvp[3][2], img.rect.mvp[3][3], // bottom right
		    img.rect.pos[0]+img.rect.size[0], img.rect.pos[1]+img.rect.size[1], 0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 0.0f, 0.0f,   img.id,   img.rect.mvp[0][0], img.rect.mvp[0][1], img.rect.mvp[0][2], img.rect.mvp[0][3], img.rect.mvp[1][0], img.rect.mvp[1][1], img.rect.mvp[1][2], img.rect.mvp[1][3], img.rect.mvp[2][0], img.rect.mvp[2][1], img.rect.mvp[2][2], img.rect.mvp[2][3], img.rect.mvp[3][0], img.rect.mvp[3][1], img.rect.mvp[3][2], img.rect.mvp[3][3], // bottom left
		    img.rect.pos[0]+img.rect.size[0], img.rect.pos[1],                  0.0f,  img.rect.color[0], img.rect.color[1], img.rect.color[2], img.rect.color[3], 0.0f, 1.0f,   img.id,   img.rect.mvp[0][0], img.rect.mvp[0][1], img.rect.mvp[0][2], img.rect.mvp[0][3], img.rect.mvp[1][0], img.rect.mvp[1][1], img.rect.mvp[1][2], img.rect.mvp[1][3], img.rect.mvp[2][0], img.rect.mvp[2][1], img.rect.mvp[2][2], img.rect.mvp[2][3], img.rect.mvp[3][0], img.rect.mvp[3][1], img.rect.mvp[3][2], img.rect.mvp[3][3], // top left <-- anchor point
		}
	};
	/* glActiveTexture(GL_TEXTURE0+img.id); <= Only when batching */
	glGenTextures(1, &img.texture);
	glBindTexture(GL_TEXTURE_2D, img.texture);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (vec4){0.0f, 0.0f, 0.0f, 1.0f});

	if (data){
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.rect.size[0], img.rect.size[1], 0, channels, GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	    MEL_log(LOG_SUCCESS, "Successfully loaded image : {%s}", path);
	}else{
	    MEL_log(LOG_ERROR, "Failed loading image : {%s}", path);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	glBindVertexArray(Renderer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(img.rect.vertices), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Renderer->indices), Renderer->indices, GL_STATIC_DRAW);

	/* Position Attribute [x,y,z] */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/* Color Attribute [R,G,B,A] */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/* Texture Coords Attribute [x, y] */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	/* Sampler Attribute [i] */
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	/* MVP Attribute [4][4] */
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(14 * sizeof(float)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(18 * sizeof(float)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 26 * sizeof(float), (void*)(22 * sizeof(float)));
	glEnableVertexAttribArray(7);

	//glVertexAttribDivisor(4, 0);
    //glVertexAttribDivisor(5, 0);
    //glVertexAttribDivisor(6, 0);
    //glVertexAttribDivisor(7, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return img;
}

MEL_Rect image_update_image(MEL_Texture source){
	MEL_Rect img = {
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
    	    source.rect.pos[0],                     source.rect.pos[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 0.0f, 0.0f, source.id, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // bottom left
    	    source.rect.pos[0],                     source.rect.pos[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 0.0f, 1.0f, source.id, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // top left <-- anchor point
    	    source.rect.pos[0]+source.rect.size[0], source.rect.pos[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 1.0f, 1.0f, source.id, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // top right
    	    source.rect.pos[0]+source.rect.size[0], source.rect.pos[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 1.0f, 0.0f, source.id, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // bottom right
		}
	};
	glm_mat4_copy(source.rect.mvp, img.mvp);
	glm_mat4_copy(source.rect.model, img.model);
	return img;
}
