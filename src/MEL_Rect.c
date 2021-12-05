#include "../include/MEL_def.h"
#include "../include/MEL_Rect.h"

MEL_Rect MEL_load_rect(MEL_Renderer2D Renderer){
	MEL_Rect r = {
		.view = GLM_MAT4_IDENTITY_INIT,
		.model = GLM_MAT4_IDENTITY_INIT,
		.pos[0] = 0.0f,
		.pos[1] = 0.0f,
		.color[0] = 1.0f,
		.color[1] = 1.0f,
		.color[2] = 1.0f,
		.color[3] = 1.0f,
		.rotation = 0.0f,
		.size[0] = 10.0f,
		.size[1] = 10.0f,
		.vertices = {
			// positions                                   // colors                                        mvp **                                                  **  **                                                      **  **                      ** **                                                        **
			r.pos[0]+r.size[0], r.pos[1],           0.0f,  r.color[0], r.color[1], r.color[2], r.color[3],  r.mvp[0][0], r.mvp[0][1], r.mvp[0][2], r.mvp[0][3], r.mvp[1][0], r.mvp[1][1], r.mvp[1][2], r.mvp[1][3], r.mvp[2][0], r.mvp[2][1], r.mvp[2][2], r.mvp[2][3], r.mvp[3][0], r.mvp[3][1], r.mvp[3][2], r.mvp[3][3], // top right
			r.pos[0]+r.size[0], r.pos[1]+r.size[1], 0.0f,  r.color[0], r.color[1], r.color[2], r.color[3],  r.mvp[0][0], r.mvp[0][1], r.mvp[0][2], r.mvp[0][3], r.mvp[1][0], r.mvp[1][1], r.mvp[1][2], r.mvp[1][3], r.mvp[2][0], r.mvp[2][1], r.mvp[2][2], r.mvp[2][3], r.mvp[3][0], r.mvp[3][1], r.mvp[3][2], r.mvp[3][3], // bottom right
			r.pos[0],           r.pos[1]+r.size[1], 0.0f,  r.color[0], r.color[1], r.color[2], r.color[3],  r.mvp[0][0], r.mvp[0][1], r.mvp[0][2], r.mvp[0][3], r.mvp[1][0], r.mvp[1][1], r.mvp[1][2], r.mvp[1][3], r.mvp[2][0], r.mvp[2][1], r.mvp[2][2], r.mvp[2][3], r.mvp[3][0], r.mvp[3][1], r.mvp[3][2], r.mvp[3][3], // bottom left
			r.pos[0],           r.pos[1],           0.0f,  r.color[0], r.color[1], r.color[2], r.color[3],  r.mvp[0][0], r.mvp[0][1], r.mvp[0][2], r.mvp[0][3], r.mvp[1][0], r.mvp[1][1], r.mvp[1][2], r.mvp[1][3], r.mvp[2][0], r.mvp[2][1], r.mvp[2][2], r.mvp[2][3], r.mvp[3][0], r.mvp[3][1], r.mvp[3][2], r.mvp[3][3], // top left <-- anchor point
		},
	};
	glBindVertexArray(Renderer.rect_items.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.rect_items.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(r.vertices), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.rect_items.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Renderer.rect_items.indices), Renderer.rect_items.indices, GL_STATIC_DRAW);

	/* Position Attribute [x,y,z] */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 23 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/* Color Attribute [R,G,B,A] */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/* MVP Attribute [4][4] */
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float), (void*)(15 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float), (void*)(19 * sizeof(float)));
	glEnableVertexAttribArray(5);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return r;
}

MEL_Rect rect_update_rect(MEL_Rect rect){
	MEL_Rect r = {
		.pos[0] = rect.pos[0],
		.pos[1] = rect.pos[1],
		.size[0]  = rect.size[0],
		.size[1]  = rect.size[1],
		.color[0] = rect.color[0],
		.color[1] = rect.color[1],
		.color[2] = rect.color[2],
		.color[3] = rect.color[3],
		.rotation = rect.rotation,
    	.vertices = {
    	    // positions                                               // colors                                                    mvp **                                                  **  **                                                      **  **                                       ** **                                                                                    **
    	    rect.pos[0],              rect.pos[1]+rect.size[1], 0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3],  rect.mvp[0][0], rect.mvp[0][1], rect.mvp[0][2], rect.mvp[0][3], rect.mvp[1][0], rect.mvp[1][1], rect.mvp[1][2], rect.mvp[1][3], rect.mvp[2][0], rect.mvp[2][1], rect.mvp[2][2], rect.mvp[2][3], rect.mvp[3][0], rect.mvp[3][1], rect.mvp[3][2], rect.mvp[3][3], // bottom left
    	    rect.pos[0],              rect.pos[1],              0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3],  rect.mvp[0][0], rect.mvp[0][1], rect.mvp[0][2], rect.mvp[0][3], rect.mvp[1][0], rect.mvp[1][1], rect.mvp[1][2], rect.mvp[1][3], rect.mvp[2][0], rect.mvp[2][1], rect.mvp[2][2], rect.mvp[2][3], rect.mvp[3][0], rect.mvp[3][1], rect.mvp[3][2], rect.mvp[3][3], // top left <-- anchor point
    	    rect.pos[0]+rect.size[0], rect.pos[1],              0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3],  rect.mvp[0][0], rect.mvp[0][1], rect.mvp[0][2], rect.mvp[0][3], rect.mvp[1][0], rect.mvp[1][1], rect.mvp[1][2], rect.mvp[1][3], rect.mvp[2][0], rect.mvp[2][1], rect.mvp[2][2], rect.mvp[2][3], rect.mvp[3][0], rect.mvp[3][1], rect.mvp[3][2], rect.mvp[3][3], // top right
    	    rect.pos[0]+rect.size[0], rect.pos[1]+rect.size[1], 0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3],  rect.mvp[0][0], rect.mvp[0][1], rect.mvp[0][2], rect.mvp[0][3], rect.mvp[1][0], rect.mvp[1][1], rect.mvp[1][2], rect.mvp[1][3], rect.mvp[2][0], rect.mvp[2][1], rect.mvp[2][2], rect.mvp[2][3], rect.mvp[3][0], rect.mvp[3][1], rect.mvp[3][2], rect.mvp[3][3], // bottom right
		}
	};
	return r;
}
