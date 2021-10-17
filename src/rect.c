#include "../include/MEL_rect.h"

#if __WIN32
MEL_Rect rect_load_rect(HANDLE hConsole, WORD saved_attributes, MEL_Renderer2D Renderer){
#else
MEL_Rect rect_load_rect(MEL_Renderer2D Renderer){
#endif
	MEL_Rect r = {
		.pos[0] = 0.0f,
		.pos[1] = 0.0f,
		.color[0] = 1.0f,
		.color[1] = 1.0f,
		.color[2] = 1.0f,
		.rotation = 0.0f,
		.size[0] = 10.0f,
		.size[1] = 10.0f,
		.vertices = {
		    // positions                                   // colors
		    r.pos[0]+r.size[0], r.pos[1],           0.0f,  r.color[0], r.color[1], r.color[2], // top right
		    r.pos[0]+r.size[0], r.pos[1]+r.size[1], 0.0f,  r.color[0], r.color[1], r.color[2], // bottom right
		    r.pos[0],           r.pos[1]+r.size[1], 0.0f,  r.color[0], r.color[1], r.color[2], // bottom left
		    r.pos[0],           r.pos[1],           0.0f,  r.color[0], r.color[1], r.color[2], // top left <-- anchor point
		},
		.indices = {
		    0, 1, 3, // first triangle
		    1, 2, 3  // second triangle
		},
	};
	glBindVertexArray(Renderer.VAO);\
	glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);\
	glBufferData(GL_ARRAY_BUFFER, sizeof(r.vertices), r.vertices, GL_STATIC_DRAW);\
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);\
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(r.indices), r.indices, GL_STATIC_DRAW);\
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);\
	glEnableVertexAttribArray(0);\
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));\
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
		.rotation = rect.rotation,
    	.vertices = {
    	    // positions                                               // colors                                     // texture coords
    	    rect.pos[0],              rect.pos[1]+rect.size[1], 0.0f,  rect.color[0], rect.color[1], rect.color[2],  0.0f, 0.0f, // bottom left
    	    rect.pos[0],              rect.pos[1],              0.0f,  rect.color[0], rect.color[1], rect.color[2],  0.0f, 1.0f,  // top left <-- anchor point
    	    rect.pos[0]+rect.size[0], rect.pos[1],              0.0f,  rect.color[0], rect.color[1], rect.color[2],  1.0f, 1.0f, // top right
    	    rect.pos[0]+rect.size[0], rect.pos[1]+rect.size[1], 0.0f,  rect.color[0], rect.color[1], rect.color[2],  1.0f, 0.0f, // bottom right
		}
	};
	return r;
}