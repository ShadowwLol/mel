#include "../include/MEL_Renderer2D.h"
#include "../include/MEL_Texture.h"

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window win){
	MEL_Renderer2D Renderer = {
		.default_texture = (GLuint *)calloc(1, sizeof(GLuint)),
		.ID = 0,
		.shader = MEL_shader(TEXTURE_VERT_SHADER_PATH, TEXTURE_FRAG_SHADER_PATH),
		.indices = {
			0, 1, 3,
			1, 2, 3
		},
	};
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Renderer.MAX_TEXTURES);
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
	glBindTexture(GL_TEXTURE_2D, 0);

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
	return cr;
}

static void MEL_send_rect(MEL_Renderer2D * Renderer, MEL_ColorRect cr){
	const uint8_t rect_ID = 0;

	/* bottom left */
	Renderer->vertices[0] = cr.pos[0];
	Renderer->vertices[1] = cr.pos[1] + cr.size[1];
	Renderer->vertices[2] = 0.0f;
	Renderer->vertices[3] = cr.color[0];
	Renderer->vertices[4] = cr.color[1];
	Renderer->vertices[5] = cr.color[2];
	Renderer->vertices[6] = cr.color[3];
	Renderer->vertices[7] = 0.0f;
	Renderer->vertices[8] = 0.0f;
	Renderer->vertices[9] = rect_ID;
	Renderer->vertices[10] = cr.mvp[0][0];
	Renderer->vertices[11] = cr.mvp[0][1];
	Renderer->vertices[12] = cr.mvp[0][2];
	Renderer->vertices[13] = cr.mvp[0][3];
	Renderer->vertices[14] = cr.mvp[1][0];
	Renderer->vertices[15] = cr.mvp[1][1];
	Renderer->vertices[16] = cr.mvp[1][2];
	Renderer->vertices[17] = cr.mvp[1][3];
	Renderer->vertices[18] = cr.mvp[2][0];
	Renderer->vertices[19] = cr.mvp[2][1];
	Renderer->vertices[20] = cr.mvp[2][2];
	Renderer->vertices[21] = cr.mvp[2][3];
	Renderer->vertices[22] = cr.mvp[3][0];
	Renderer->vertices[23] = cr.mvp[3][1];
	Renderer->vertices[24] = cr.mvp[3][2];
	Renderer->vertices[25] = cr.mvp[3][3];
	/* * * * * * * */

	/* top left  */
	Renderer->vertices[26] = cr.pos[0];
	Renderer->vertices[27] = cr.pos[1];
	Renderer->vertices[28] = 0.0f;
	Renderer->vertices[29] = cr.color[0];
	Renderer->vertices[30] = cr.color[1];
	Renderer->vertices[31] = cr.color[2];
	Renderer->vertices[32] = cr.color[3];
	Renderer->vertices[33] = 0.0f;
	Renderer->vertices[34] = 1.0f;
	Renderer->vertices[35] = rect_ID;
	Renderer->vertices[36] = cr.mvp[0][0];
	Renderer->vertices[37] = cr.mvp[0][1];
	Renderer->vertices[38] = cr.mvp[0][2];
	Renderer->vertices[39] = cr.mvp[0][3];
	Renderer->vertices[40] = cr.mvp[1][0];
	Renderer->vertices[41] = cr.mvp[1][1];
	Renderer->vertices[42] = cr.mvp[1][2];
	Renderer->vertices[43] = cr.mvp[1][3];
	Renderer->vertices[44] = cr.mvp[2][0];
	Renderer->vertices[45] = cr.mvp[2][1];
	Renderer->vertices[46] = cr.mvp[2][2];
	Renderer->vertices[47] = cr.mvp[2][3];
	Renderer->vertices[48] = cr.mvp[3][0];
	Renderer->vertices[49] = cr.mvp[3][1];
	Renderer->vertices[50] = cr.mvp[3][2];
	Renderer->vertices[51] = cr.mvp[3][3];
	/* * * * * * */

	/* top right */
	Renderer->vertices[52] = cr.pos[0]+cr.size[0];
	Renderer->vertices[53] = cr.pos[1];
	Renderer->vertices[54] = 0.0f;
	Renderer->vertices[55] = cr.color[0];
	Renderer->vertices[56] = cr.color[1];
	Renderer->vertices[57] = cr.color[2];
	Renderer->vertices[58] = cr.color[3];
	Renderer->vertices[59] = 1.0f;
	Renderer->vertices[60] = 1.0f;
	Renderer->vertices[61] = rect_ID;
	Renderer->vertices[62] = cr.mvp[0][0];
	Renderer->vertices[63] = cr.mvp[0][1];
	Renderer->vertices[64] = cr.mvp[0][2];
	Renderer->vertices[65] = cr.mvp[0][3];
	Renderer->vertices[66] = cr.mvp[1][0];
	Renderer->vertices[67] = cr.mvp[1][1];
	Renderer->vertices[68] = cr.mvp[1][2];
	Renderer->vertices[69] = cr.mvp[1][3];
	Renderer->vertices[70] = cr.mvp[2][0];
	Renderer->vertices[71] = cr.mvp[2][1];
	Renderer->vertices[72] = cr.mvp[2][2];
	Renderer->vertices[73] = cr.mvp[2][3];
	Renderer->vertices[74] = cr.mvp[3][0];
	Renderer->vertices[75] = cr.mvp[3][1];
	Renderer->vertices[76] = cr.mvp[3][2];
	Renderer->vertices[77] = cr.mvp[3][3];
	/* * * * * * */

	/* bottom right  */
	Renderer->vertices[78] = cr.pos[0]+cr.size[0];
	Renderer->vertices[79] = cr.pos[1]+cr.size[1];
	Renderer->vertices[80] = 0.0f;
	Renderer->vertices[81] = cr.color[0];
	Renderer->vertices[82] = cr.color[1];
	Renderer->vertices[83] = cr.color[2];
	Renderer->vertices[84] = cr.color[3];
	Renderer->vertices[85] = 1.0f;
	Renderer->vertices[86] = 0.0f;
	Renderer->vertices[87] = rect_ID;
	Renderer->vertices[88] = cr.mvp[0][0];
	Renderer->vertices[89] = cr.mvp[0][1];
	Renderer->vertices[90] = cr.mvp[0][2];
	Renderer->vertices[91] = cr.mvp[0][3];
	Renderer->vertices[92] = cr.mvp[1][0];
	Renderer->vertices[93] = cr.mvp[1][1];
	Renderer->vertices[94] = cr.mvp[1][2];
	Renderer->vertices[95] = cr.mvp[1][3];
	Renderer->vertices[96] = cr.mvp[2][0];
	Renderer->vertices[97] = cr.mvp[2][1];
	Renderer->vertices[98] = cr.mvp[2][2];
	Renderer->vertices[99] = cr.mvp[2][3];
	Renderer->vertices[100] = cr.mvp[3][0];
	Renderer->vertices[101] = cr.mvp[3][1];
	Renderer->vertices[102] = cr.mvp[3][2];
	Renderer->vertices[103] = cr.mvp[3][3];
	/* * * * * * * * */

}

void MEL_draw_rect(MEL_Window MELW, MEL_Renderer2D * Renderer, MEL_ColorRect * Rect, MEL_Camera Camera){
	if (((Rect->pos[0] < MELW.mode->width) && ((Rect->pos[0]+Rect->size[0]) > 0)) &&\
	((Rect->pos[1] < MELW.mode->height) && ((Rect->pos[1]+Rect->size[1]) > 0))){

		MEL_send_rect(Renderer, *Rect);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Renderer->vertices), Renderer->vertices);
		glm_mat4_identity(Rect->model);
		glm_rotate_at(Rect->model, (vec3){(float)(Rect->pos[0] + Rect->size[0]/2.0f), (float)(Rect->pos[1] + Rect->size[1]/2.0f), 0.0f}, glm_rad(Rect->rotation), (vec3){0.0f, 0.0f, 1.0f});
		glm_mat4_mul(Renderer->projection, Camera.view, Rect->mvp);
		glm_mat4_mul(Rect->mvp, Rect->model, Rect->mvp);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *Renderer->default_texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}
}
