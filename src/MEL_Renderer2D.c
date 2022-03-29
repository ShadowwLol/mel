#include "../include/MEL_Renderer2D.h"
#include "../include/MEL_Texture.h"

extern MEL_ctx mctx;

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window win){
	MEL_Renderer2D Renderer = {
		.default_texture = (GLuint *)calloc(1, sizeof(GLuint)),
		.tex_count = 0,
		.ID = 1,
		.shader = MEL_shader(TEXTURE_VERT_SHADER_PATH, TEXTURE_FRAG_SHADER_PATH),
	};

	/* generate_indice_array(&Renderer.indices); */
	uint32_t offset = 0;
	for (size_t i = 0; i < MAX_INDEX_COUNT; i += 6, offset += 4){
		Renderer.indices[i + 0] = 0 + offset;
		Renderer.indices[i + 1] = 1 + offset;
		Renderer.indices[i + 2] = 2 + offset;

		Renderer.indices[i + 3] = 2 + offset;
		Renderer.indices[i + 4] = 3 + offset;
		Renderer.indices[i + 5] = 0 + offset;
	}

	/* TODO: */
	/* Create texture atlas instead of binding multiple textures in different places, just binding the atlas once*/

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Renderer.max_tex);
	glm_ortho(0.0f, (float)win.mode->width, (float)win.mode->height, 0.0f, -1.0f, 1.0f, Renderer.projection);

	glGenTextures(1, Renderer.default_texture);
	glBindTexture(GL_TEXTURE_2D, *Renderer.default_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (vec4){0.0f, 0.0f, 0.0f, 1.0f});

	uint32_t tex_color = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex_color);

	glGenVertexArrays(1, &Renderer.VAO);
	glGenBuffers(1, &Renderer.VBO);
	glGenBuffers(1, &Renderer.EBO);

	glBindVertexArray(Renderer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer.vertices), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Renderer.indices), Renderer.indices, GL_STATIC_DRAW);

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

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return Renderer;
}

MEL_ColorRect MEL_init_rect(MEL_Renderer2D * Renderer){
	MEL_ColorRect cr = {
		.id = (Renderer->tex_count < Renderer->max_tex) ? ++Renderer->tex_count : 1,
		.mvp = GLM_MAT4_IDENTITY_INIT,
		.model = GLM_MAT4_IDENTITY_INIT,
		.pos[0] = 0.0f,
		.pos[1] = 0.0f,
		.size[0] = 1,
		.size[1] = 1,
		.color[0] = 1.0f,
		.color[1] = 1.0f,
		.color[2] = 1.0f,
		.color[3] = 1.0f,
		.rotation = 0.0f,
	};

	if (Renderer->tex_count >= Renderer->max_tex){
		Renderer->tex_count = 1;
	}

	return cr;
}

static void MEL_send_rect(MEL_Renderer2D * Renderer, MEL_ColorRect cr){
	const uint32_t rect_ID = 0;

	/* bottom left */
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+0] = cr.pos[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+1] = cr.pos[1] + cr.size[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+2] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+3] = cr.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+4] = cr.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+5] = cr.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+6] = cr.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+7] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+8] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+9] = rect_ID;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+10] = cr.mvp[0][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+11] = cr.mvp[0][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+12] = cr.mvp[0][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+13] = cr.mvp[0][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+14] = cr.mvp[1][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+15] = cr.mvp[1][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+16] = cr.mvp[1][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+17] = cr.mvp[1][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+18] = cr.mvp[2][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+19] = cr.mvp[2][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+20] = cr.mvp[2][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+21] = cr.mvp[2][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+22] = cr.mvp[3][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+23] = cr.mvp[3][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+24] = cr.mvp[3][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+25] = cr.mvp[3][3];
	/* * * * * * * */

	/* top left  */
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+26] = cr.pos[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+27] = cr.pos[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+28] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+29] = cr.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+30] = cr.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+31] = cr.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+32] = cr.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+33] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+34] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+35] = rect_ID;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+36] = cr.mvp[0][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+37] = cr.mvp[0][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+38] = cr.mvp[0][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+39] = cr.mvp[0][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+40] = cr.mvp[1][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+41] = cr.mvp[1][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+42] = cr.mvp[1][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+43] = cr.mvp[1][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+44] = cr.mvp[2][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+45] = cr.mvp[2][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+46] = cr.mvp[2][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+47] = cr.mvp[2][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+48] = cr.mvp[3][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+49] = cr.mvp[3][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+50] = cr.mvp[3][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+51] = cr.mvp[3][3];
	/* * * * * * */

	/* top right */
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+52] = cr.pos[0]+cr.size[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+53] = cr.pos[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+54] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+55] = cr.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+56] = cr.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+57] = cr.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+58] = cr.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+59] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+60] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+61] = rect_ID;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+62] = cr.mvp[0][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+63] = cr.mvp[0][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+64] = cr.mvp[0][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+65] = cr.mvp[0][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+66] = cr.mvp[1][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+67] = cr.mvp[1][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+68] = cr.mvp[1][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+69] = cr.mvp[1][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+70] = cr.mvp[2][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+71] = cr.mvp[2][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+72] = cr.mvp[2][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+73] = cr.mvp[2][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+74] = cr.mvp[3][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+75] = cr.mvp[3][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+76] = cr.mvp[3][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+77] = cr.mvp[3][3];
	/* * * * * * */

	/* bottom right  */
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+78] = cr.pos[0]+cr.size[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+79] = cr.pos[1]+cr.size[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+80] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+81] = cr.color[0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+82] = cr.color[1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+83] = cr.color[2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+84] = cr.color[3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+85] = 1.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+86] = 0.0f;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+87] = rect_ID;
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+88] = cr.mvp[0][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+89] = cr.mvp[0][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+90] = cr.mvp[0][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+91] = cr.mvp[0][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+92] = cr.mvp[1][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+93] = cr.mvp[1][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+94] = cr.mvp[1][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+95] = cr.mvp[1][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+96] = cr.mvp[2][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+97] = cr.mvp[2][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+98] = cr.mvp[2][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+99] = cr.mvp[2][3];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+100] = cr.mvp[3][0];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+101] = cr.mvp[3][1];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+102] = cr.mvp[3][2];
	Renderer->vertices[(VERTEX_COUNT * Renderer->ID)+103] = cr.mvp[3][3];
	/* * * * * * * * */

	++Renderer->ID;
}

void MEL_draw_rect(MEL_Window MELW, MEL_Renderer2D * Renderer, MEL_ColorRect * Rect, MEL_Camera Camera){
	if (((Rect->pos[0] < MELW.mode->width) && ((Rect->pos[0]+Rect->size[0]) > 0)) &&\
	((Rect->pos[1] < MELW.mode->height) && ((Rect->pos[1]+Rect->size[1]) > 0))){

		MEL_send_rect(Renderer, *Rect);
		glm_mat4_identity(Rect->model);
		glm_rotate_at(Rect->model, (vec3){(float)(Rect->pos[0] + Rect->size[0]/2.0f), (float)(Rect->pos[1] + Rect->size[1]/2.0f), 0.0f}, glm_rad(Rect->rotation), (vec3){0.0f, 0.0f, 1.0f});
		glm_mat4_mul(Renderer->projection, Camera.view, Rect->mvp);
		glm_mat4_mul(Rect->mvp, Rect->model, Rect->mvp);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *Renderer->default_texture);
	}
}

void MEL_begin2D(MEL_Renderer2D * Renderer){
	glBindVertexArray(Renderer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer->EBO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(Renderer->shader);
}

void MEL_end2D(MEL_Renderer2D * Renderer){
	glBufferSubData(GL_ARRAY_BUFFER, 0, ((sizeof(Renderer->vertices[0]) * VERTEX_COUNT) * Renderer->ID), Renderer->vertices);
	GLint loc = glGetUniformLocation(Renderer->shader, "u_Textures");
	int32_t samplers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	glUniform1iv(loc, 10, samplers);
	glDrawElements(GL_TRIANGLES, (Renderer->ID * INDEX_COUNT), GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glfwSwapBuffers(mctx.window_ctx.window);
	Renderer->ID = 1;
}

void MEL_Renderer2D_destroy(MEL_Renderer2D * Renderer){
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &Renderer->VAO);
	glDeleteBuffers(1, &Renderer->VBO);
	glDeleteBuffers(1, &Renderer->EBO);
	glDeleteProgram(Renderer->shader);

}
