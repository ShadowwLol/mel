#include "../include/MEL_Renderer2D.h"
#include "../include/MEL_Texture.h"

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window win){
	MEL_Renderer2D Renderer = {
		.default_texture = (GLuint *)calloc(1, sizeof(GLuint)),
		/* Image */
		.TEXTURE_COUNT = 1,
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

	/* Image */
	glGenVertexArrays(1, &Renderer.VAO);
	glGenBuffers(1, &Renderer.VBO);
	glGenBuffers(1, &Renderer.EBO);
	/* Rectangle */
	glGenVertexArrays(1, &Renderer.VAO);
	glGenBuffers(1, &Renderer.VBO);
	glGenBuffers(1, &Renderer.EBO);
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

	glBindVertexArray(Renderer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cr.vertices), NULL, GL_DYNAMIC_DRAW);
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

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return cr;
}
MEL_ColorRect MEL_update_rect(MEL_ColorRect cr){
	MEL_ColorRect r = {
		.pos[0]   = cr.pos[0],
		.pos[1]   = cr.pos[1],
		.size[0]  = cr.size[0],
		.size[1]  = cr.size[1],
		.color[0] = cr.color[0],
		.color[1] = cr.color[1],
		.color[2] = cr.color[2],
		.color[3] = cr.color[3],
		.rotation = cr.rotation,
		.vertices = {
		    // positions                                                               colors                                 tex coords  sampler   mvp **                                                  **  **                                                      **  **                                                   ** **                                                       **
		    cr.pos[0],            cr.pos[1],            0.0f,  cr.color[0], cr.color[1], cr.color[2], cr.color[3], 1.0f, 1.0f,  0,  cr.mvp[0][0], cr.mvp[0][1], cr.mvp[0][2], cr.mvp[0][3], cr.mvp[1][0], cr.mvp[1][1], cr.mvp[1][2], cr.mvp[1][3], cr.mvp[2][0], cr.mvp[2][1], cr.mvp[2][2], cr.mvp[2][3], cr.mvp[3][0], cr.mvp[3][1], cr.mvp[3][2], cr.mvp[3][3], // top right
		    cr.pos[0],            cr.pos[1]+cr.size[1], 0.0f,  cr.color[0], cr.color[1], cr.color[2], cr.color[3], 1.0f, 0.0f,  0,  cr.mvp[0][0], cr.mvp[0][1], cr.mvp[0][2], cr.mvp[0][3], cr.mvp[1][0], cr.mvp[1][1], cr.mvp[1][2], cr.mvp[1][3], cr.mvp[2][0], cr.mvp[2][1], cr.mvp[2][2], cr.mvp[2][3], cr.mvp[3][0], cr.mvp[3][1], cr.mvp[3][2], cr.mvp[3][3], // bottom right
		    cr.pos[0]+cr.size[0], cr.pos[1]+cr.size[1], 0.0f,  cr.color[0], cr.color[1], cr.color[2], cr.color[3], 0.0f, 0.0f,  0,  cr.mvp[0][0], cr.mvp[0][1], cr.mvp[0][2], cr.mvp[0][3], cr.mvp[1][0], cr.mvp[1][1], cr.mvp[1][2], cr.mvp[1][3], cr.mvp[2][0], cr.mvp[2][1], cr.mvp[2][2], cr.mvp[2][3], cr.mvp[3][0], cr.mvp[3][1], cr.mvp[3][2], cr.mvp[3][3], // bottom left
		    cr.pos[0]+cr.size[0], cr.pos[1],            0.0f,  cr.color[0], cr.color[1], cr.color[2], cr.color[3], 0.0f, 1.0f,  0,  cr.mvp[0][0], cr.mvp[0][1], cr.mvp[0][2], cr.mvp[0][3], cr.mvp[1][0], cr.mvp[1][1], cr.mvp[1][2], cr.mvp[1][3], cr.mvp[2][0], cr.mvp[2][1], cr.mvp[2][2], cr.mvp[2][3], cr.mvp[3][0], cr.mvp[3][1], cr.mvp[3][2], cr.mvp[3][3], // top left <-- anchor point
		}
	};
	glm_mat4_copy(cr.mvp,   r.mvp);
	glm_mat4_copy(cr.model, r.model);

	return r;
}
