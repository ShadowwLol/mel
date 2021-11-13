#include "../include/MEL_def.h"
#include "../include/MEL_rect.h"

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
		    // positions                                   // colors
		    r.pos[0]+r.size[0], r.pos[1],           0.0f,  r.color[0], r.color[1], r.color[2], r.color[3], // top right
		    r.pos[0]+r.size[0], r.pos[1]+r.size[1], 0.0f,  r.color[0], r.color[1], r.color[2], r.color[3], // bottom right
		    r.pos[0],           r.pos[1]+r.size[1], 0.0f,  r.color[0], r.color[1], r.color[2], r.color[3], // bottom left
		    r.pos[0],           r.pos[1],           0.0f,  r.color[0], r.color[1], r.color[2], r.color[3], // top left <-- anchor point
		},
	};
	glBindVertexArray(Renderer.rect_items.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.rect_items.VBO);\
	glBufferData(GL_ARRAY_BUFFER, sizeof(r.vertices), r.vertices, GL_STATIC_DRAW);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.rect_items.EBO);\
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Renderer.rect_items.indices), Renderer.rect_items.indices, GL_STATIC_DRAW);\

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);\
	glEnableVertexAttribArray(0);\
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));\
	glEnableVertexAttribArray(1);\

	glBindVertexArray(0);\
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);\

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
    	    // positions                                               // colors
    	    rect.pos[0],              rect.pos[1]+rect.size[1], 0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3], // bottom left
    	    rect.pos[0],              rect.pos[1],              0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3], // top left <-- anchor point
    	    rect.pos[0]+rect.size[0], rect.pos[1],              0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3], // top right
    	    rect.pos[0]+rect.size[0], rect.pos[1]+rect.size[1], 0.0f,  rect.color[0], rect.color[1], rect.color[2], rect.color[3], // bottom right
		}
	};
	return r;
}
