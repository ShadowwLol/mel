#include "inc/def.h"
#include "inc/logs.h"
#include "inc/texture.h"

MEL_Texture MEL_load_tex(MEL_Renderer2D * Renderer, GLchar * path, GLenum channels, GLenum min_filter, GLenum max_filter){
	stbi_set_flip_vertically_on_load(true);
	GLint w, h, c;
	stbi_uc * data = stbi_load(path, &w, &h, &c, 0);
	MEL_Texture img = {
		.id = (Renderer->tex_count < Renderer->max_tex) ? ++Renderer->tex_count : 1,
		.rect.mvp = GLM_MAT4_IDENTITY_INIT,
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

	if (Renderer->tex_count >= Renderer->max_tex){
		Renderer->tex_count = 1;
	}

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
		MEL_log(LOG_SUCCESS, "Successfully loaded image: {%s}", path);
	}else{
		MEL_log(LOG_ERROR, "Failed loading image: {%s}", path);
	}
	stbi_image_free(data);

	return img;
}

static void MEL_send_tex(MEL_Renderer2D * Renderer, MEL_Texture source){
	vec4 coords, result = GLM_VEC4_ZERO_INIT;
	coords[2] = 0.0f;
	coords[3] = 1.0f;

	const uint32_t source_ID = source.id;

	/* bottom left */
	coords[0] = source.rect.pos[0];
	coords[1] = source.rect.pos[1] + source.rect.size[1];
	glm_mat4_mulv(source.rect.mvp, coords, result);
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+0] = result[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+1] = result[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+2] = source.rect.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+3] = source.rect.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+4] = source.rect.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+5] = source.rect.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+6] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+7] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+8] = source_ID;
	/* * * * * * * */

	/* top left  */
	coords[0] = source.rect.pos[0];
	coords[1] = source.rect.pos[1];
	glm_mat4_mulv(source.rect.mvp, coords, result);
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+9]  = result[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+10] = result[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+11] = source.rect.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+12] = source.rect.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+13] = source.rect.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+14] = source.rect.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+15] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+16] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+17] = source_ID;
	/* * * * * * */

	/* top right */
	coords[0] = source.rect.pos[0] + source.rect.size[0];
	coords[1] = source.rect.pos[1];
	glm_mat4_mulv(source.rect.mvp, coords, result);
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+18] = result[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+19] = result[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+20] = source.rect.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+21] = source.rect.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+22] = source.rect.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+23] = source.rect.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+24] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+25] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+26] = source_ID;
	/* * * * * * */

	/* bottom right  */
	coords[0] = source.rect.pos[0] + source.rect.size[0];
	coords[1] = source.rect.pos[1] + source.rect.size[1];
	glm_mat4_mulv(source.rect.mvp, coords, result);
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+27] = result[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+28] = result[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+29] = source.rect.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+30] = source.rect.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+31] = source.rect.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+32] = source.rect.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+33] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+34] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+35] = source_ID;
	/* * * * * * * * */

	++Renderer->ID;
}

void MEL_draw_tex(MEL_ctx ctx, MEL_Renderer2D * Renderer, MEL_Texture * Img, MEL_Camera Camera){
	if (is_visible(ctx, Img->rect, Camera)){
		if (Renderer->ID >= MAX_QUAD_COUNT){
			glBufferSubData(GL_ARRAY_BUFFER, 0, ((sizeof(Renderer->vertices[0]) * VERTEX_COUNT) * Renderer->ID), Renderer->vertices);
			GLint loc = glGetUniformLocation(Renderer->shader, "u_Textures");
			int32_t samplers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
			glUniform1iv(loc, 10, samplers);
			glDrawElements(GL_TRIANGLES, (Renderer->ID * INDEX_COUNT), GL_UNSIGNED_INT, 0);
			Renderer->ID = 1;
		}

		MEL_send_tex(Renderer, *Img);
		glm_mat4_mul(Renderer->projection, Camera.view, Img->rect.mvp);
		glm_rotate_at(Img->rect.mvp, (vec3){(float)(Img->rect.pos[0] + Img->rect.size[0]/2.0f), (float)(Img->rect.pos[1] + Img->rect.size[1]/2.0f), 0.0f}, glm_rad(Img->rect.rotation), (vec3){0.0f, 0.0f, 1.0f});
		/* https://stackoverflow.com/a/28360579/16946028 */
		/* glActiveTexture(GL_TEXTURE0+Img.id); <= Only when batching */
		glActiveTexture(GL_TEXTURE0 + Img->id);
		glBindTexture(GL_TEXTURE_2D, Img->texture);

	}
}
