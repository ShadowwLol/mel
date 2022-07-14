#ifndef _MEL_RENDER_H
#define _MEL_RENDER_H

#include "mel.h"
#include "shaders.h"
#include "camera.h"

#define TEXTURE_VERT_SHADER_PATH "resources/shaders/texture.vert"
#define TEXTURE_FRAG_SHADER_PATH "resources/shaders/texture.frag"

#define VERTEX_COUNT (36)
#define INDEX_COUNT (6)

#define MAX_QUAD_COUNT  (1000)
#define MAX_VERTEX_COUNT (VERTEX_COUNT * MAX_QUAD_COUNT)
#define MAX_INDEX_COUNT  (INDEX_COUNT * MAX_QUAD_COUNT)

typedef struct {
  mat4 projection;
  GLuint* default_texture;

  GLuint VAO, VBO, EBO, shader;
  GLint ID, tex_count, max_tex;
  GLuint indices[MAX_INDEX_COUNT];
  GLfloat vertices[MAX_VERTEX_COUNT];
} MEL_Renderer2D;

MEL_Renderer2D MEL_Renderer2D_init(mel_t);

void MEL_begin2D(MEL_Renderer2D * Renderer);
void MEL_end2D(mel_t * ctx, MEL_Renderer2D * Renderer);
void MEL_Renderer2D_destroy(MEL_Renderer2D * Renderer);

#endif
