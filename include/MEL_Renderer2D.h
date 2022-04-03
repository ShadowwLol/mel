#ifndef _MEL_RENDER_H
#define _MEL_RENDER_H

#include "MEL_opengl.h"
#include "MEL_def.h"
#include "MEL_shader.h"
#include "MEL_Camera.h"

#define TEXTURE_VERT_SHADER_PATH "resources/shaders/image.vert"
#define TEXTURE_FRAG_SHADER_PATH "resources/shaders/image.frag"

#define VERTEX_COUNT (104)
#define INDEX_COUNT (6)

#define MAX_QUAD_COUNT  (1000)
#define MAX_VERTEX_COUNT (VERTEX_COUNT * MAX_QUAD_COUNT)
#define MAX_INDEX_COUNT  (INDEX_COUNT * MAX_QUAD_COUNT)

typedef struct{
	GLuint id;
	vec2 pos;
	vec2 size;
	vec4 color;
	mat4 mvp;
	GLfloat rotation;
} MEL_Rect;

typedef MEL_Rect MEL_ColorRect;

typedef struct {
	mat4 projection;
	GLuint * default_texture;

	GLuint VAO, VBO, EBO, shader;
	GLint ID, tex_count, max_tex;
	GLuint indices[MAX_INDEX_COUNT];
	GLfloat vertices[MAX_VERTEX_COUNT];
} MEL_Renderer2D;

bool is_visible(MEL_ctx ctx, MEL_Rect rect, MEL_Camera camera);
MEL_Renderer2D MEL_Renderer2D_init(MEL_Window);
MEL_ColorRect MEL_init_rect(MEL_Renderer2D *);
void MEL_draw_rect(MEL_ctx ctx, MEL_Renderer2D * Renderer, MEL_ColorRect * Rect, MEL_Camera Camera);

void MEL_begin2D(MEL_Renderer2D * Renderer);
void MEL_end2D(MEL_Renderer2D * Renderer);
void MEL_Renderer2D_destroy(MEL_Renderer2D * Renderer);

#endif
