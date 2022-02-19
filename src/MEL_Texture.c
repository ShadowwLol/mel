#include "../include/MEL_def.h"
#include "../include/MEL_logs.h"
#include "../include/MEL_Texture.h"
#include <cglm/affine.h>
#include <cglm/util.h>

MEL_Texture MEL_load_tex(MEL_Renderer2D * Renderer, GLchar * path, GLenum channels, GLenum min_filter, GLenum max_filter){
	stbi_set_flip_vertically_on_load(true);
	GLint w, h, c;
	stbi_uc * data = stbi_load(path, &w, &h, &c, 0);
	MEL_Texture img = {
		.rect.mvp = GLM_MAT4_IDENTITY_INIT,
		.rect.model = GLM_MAT4_IDENTITY_INIT,
		.rect.pos[0] = 0.0f,
		.rect.pos[1] = 0.0f,
		.rect.size[0] = w,
		.rect.size[1] = h,
		.rect.color[0] = 1.0f,
		.rect.color[1] = 1.0f,
		.rect.color[2] = 1.0f,
		.rect.color[3] = 1.0f,
		.rect.rotation = 0.0f,
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

	return img;
}

void MEL_update_tex(MEL_Renderer2D * Renderer, MEL_Texture source){
	MEL_geometry geometry = {
		.vertices = {
    	    // positions                                                                           colors                                                                                  tex coords  sampler
    	    source.rect.pos[0],                     source.rect.pos[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 0.0f, 0.0f, source.texture, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // bottom left
    	    source.rect.pos[0],                     source.rect.pos[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 0.0f, 1.0f, source.texture, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // top left <-- anchor point
    	    source.rect.pos[0]+source.rect.size[0], source.rect.pos[1],                     0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 1.0f, 1.0f, source.texture, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // top right
    	    source.rect.pos[0]+source.rect.size[0], source.rect.pos[1]+source.rect.size[1], 0.0f,  source.rect.color[0], source.rect.color[1], source.rect.color[2], source.rect.color[3], 1.0f, 0.0f, source.texture, source.rect.mvp[0][0], source.rect.mvp[0][1], source.rect.mvp[0][2], source.rect.mvp[0][3], source.rect.mvp[1][0], source.rect.mvp[1][1], source.rect.mvp[1][2], source.rect.mvp[1][3], source.rect.mvp[2][0], source.rect.mvp[2][1], source.rect.mvp[2][2], source.rect.mvp[2][3], source.rect.mvp[3][0], source.rect.mvp[3][1], source.rect.mvp[3][2], source.rect.mvp[3][3],  // bottom right
		}
	};

	Renderer->geometry = geometry;
}
