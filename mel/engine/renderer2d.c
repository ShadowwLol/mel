#include "inc/renderer2d.h"
#include "inc/texture.h"

MEL_Renderer2D MEL_Renderer2D_init(MEL_Window win) {
  MEL_Renderer2D Renderer = {
    .default_texture = (GLuint *) calloc(1, sizeof(GLuint)),
    .tex_count = 0,
    .ID = 1,
    .shader = MEL_shader(TEXTURE_VERT_SHADER_PATH, TEXTURE_FRAG_SHADER_PATH),
  };

  /* generate_indice_array(&Renderer.indices); */
  uint32_t offset = 0;
  for (size_t i = 0; i < MAX_INDEX_COUNT; i += 6, offset += 4) {
    Renderer.indices[i + 0] = 0 + offset;
    Renderer.indices[i + 1] = 1 + offset;
    Renderer.indices[i + 2] = 2 + offset;

    Renderer.indices[i + 3] = 2 + offset;
    Renderer.indices[i + 4] = 3 + offset;
    Renderer.indices[i + 5] = 0 + offset;
  }

  /* TODO: */
  /* Create texture atlas instead of binding multiple textures in different places, just binding the atlas once */

  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Renderer.max_tex);
  glm_ortho(0.0f, (float) win.mode->width, (float) win.mode->height, 0.0f,
            -1.0f, 1.0f, Renderer.projection);

  glGenTextures(1, Renderer.default_texture);
  glBindTexture(GL_TEXTURE_2D, *Renderer.default_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (vec4) {
                   0.0f, 0.0f, 0.0f, 1.0f});

  uint32_t tex_color = 0xffffffff;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               &tex_color);

  glGenVertexArrays(1, &Renderer.VAO);
  glGenBuffers(1, &Renderer.VBO);
  glGenBuffers(1, &Renderer.EBO);

  glBindVertexArray(Renderer.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, Renderer.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer.vertices), NULL,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Renderer.indices),
               Renderer.indices, GL_STATIC_DRAW);

  /* Position Attribute [x,y] */
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *) 0);
  glEnableVertexAttribArray(0);
  /* Color Attribute [R,G,B,A] */
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *) (2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  /* Texture Coords Attribute [x, y] */
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  /* Sampler Attribute [i] */
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *) (8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return Renderer;
}

MEL_ColorRect MEL_init_rect(MEL_Renderer2D * Renderer) {
  MEL_ColorRect cr = {
    .id =
      (Renderer->tex_count < Renderer->max_tex) ? ++Renderer->tex_count : 1,
    .mvp = GLM_MAT4_IDENTITY_INIT,
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

  if (Renderer->tex_count >= Renderer->max_tex) {
    Renderer->tex_count = 1;
  }

  return cr;
}

static void MEL_send_rect(MEL_Renderer2D * Renderer, MEL_ColorRect cr) {
  vec4 coords, result = GLM_VEC4_ZERO_INIT;
  coords[2] = 0.0f;
  coords[3] = 1.0f;

  const uint32_t rect_ID = 0;

  /* bottom left */
  coords[0] = cr.pos[0];
  coords[1] = cr.pos[1] + cr.size[1];
  glm_mat4_mulv(cr.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 0] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 1] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 2] = cr.color[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 3] = cr.color[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 4] = cr.color[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 5] = cr.color[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 6] = 0.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 7] = 0.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 8] = rect_ID;
  /* * * * * * * */

  /* top left  */
  coords[0] = cr.pos[0];
  coords[1] = cr.pos[1];
  glm_mat4_mulv(cr.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 9] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 10] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 11] = cr.color[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 12] = cr.color[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 13] = cr.color[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 14] = cr.color[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 15] = 0.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 16] = 1.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 17] = rect_ID;
  /* * * * * * */

  /* top right */
  coords[0] = cr.pos[0] + cr.size[0];
  coords[1] = cr.pos[1];
  glm_mat4_mulv(cr.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 18] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 19] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 20] = cr.color[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 21] = cr.color[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 22] = cr.color[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 23] = cr.color[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 24] = 1.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 25] = 1.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 26] = rect_ID;
  /* * * * * * */

  /* bottom right  */
  coords[0] = cr.pos[0] + cr.size[0];
  coords[1] = cr.pos[1] + cr.size[1];
  glm_mat4_mulv(cr.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 27] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 28] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 29] = cr.color[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 30] = cr.color[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 31] = cr.color[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 32] = cr.color[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 33] = 1.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 34] = 0.0f;
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 35] = rect_ID;
  /* * * * * * * * */

  ++Renderer->ID;
}

void MEL_draw_rect(MEL_ctx ctx, MEL_Renderer2D * Renderer,
                   MEL_ColorRect * Rect, MEL_Camera Camera) {
  if (is_visible(ctx, *Rect, Camera)) {
    if (Renderer->ID >= MAX_QUAD_COUNT) {
      glBufferSubData(GL_ARRAY_BUFFER, 0,
                      ((sizeof(Renderer->vertices[0]) * VERTEX_COUNT) *
                       Renderer->ID), Renderer->vertices);
      GLint loc = glGetUniformLocation(Renderer->shader, "u_Textures");
      int32_t samplers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      glUniform1iv(loc, 10, samplers);
      glDrawElements(GL_TRIANGLES, (Renderer->ID * INDEX_COUNT),
                     GL_UNSIGNED_INT, 0);
      Renderer->ID = 1;
    }

    MEL_send_rect(Renderer, *Rect);
    glm_mat4_mul(Renderer->projection, Camera.view, Rect->mvp);
    glm_rotate_at(Rect->mvp, (vec3) {
                  (float) (Rect->pos[0] + Rect->size[0] / 2.0f),
                  (float) (Rect->pos[1] + Rect->size[1] / 2.0f), 0.0f},
                  glm_rad(Rect->rotation), (vec3) {
                  0.0f, 0.0f, 1.0f});
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *Renderer->default_texture);
  }
}

void MEL_begin2D(MEL_Renderer2D * Renderer) {
  glBindVertexArray(Renderer->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, Renderer->VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer->EBO);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(Renderer->shader);
}

void MEL_end2D(MEL_ctx * ctx, MEL_Renderer2D * Renderer) {
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  ((sizeof(Renderer->vertices[0]) * VERTEX_COUNT) *
                   Renderer->ID), Renderer->vertices);
  GLint loc = glGetUniformLocation(Renderer->shader, "u_Textures");
  int32_t samplers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  glUniform1iv(loc, 10, samplers);
  glDrawElements(GL_TRIANGLES, (Renderer->ID * INDEX_COUNT), GL_UNSIGNED_INT,
                 0);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glfwSwapBuffers(ctx->window_ctx.window);
  Renderer->ID = 1;
}

void MEL_Renderer2D_destroy(MEL_Renderer2D * Renderer) {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteVertexArrays(1, &Renderer->VAO);
  glDeleteBuffers(1, &Renderer->VBO);
  glDeleteBuffers(1, &Renderer->EBO);
  glDeleteProgram(Renderer->shader);

}

/* TODO: Fix function */
bool is_visible(MEL_ctx ctx, MEL_Rect rect, MEL_Camera camera) {
  return true;
#if 0
/* FIXME: Clipping when pos < 0 ? */
  const GLfloat v1 = rect.pos[0];
  const GLfloat v2 = rect.pos[1] + rect.size[1];
  const GLfloat v4 = rect.pos[1];
  const GLfloat v5 = rect.pos[0] + rect.size[0];

  mat4 m4[4] = { { v1, v2, 0.0f, 1.0f },
  { v1, v4, 0.0f, 1.0f },
  { v5, v4, 0.0f, 1.0f },
  { v5, v2, 0.0f, 1.0f }
  };

  for (uint8_t i = 0; i < 4; ++i) {
    glm_mat4_mul(m4[i], rect.mvp, m4[i]);
    if ((*m4[i][0] >= 0 && *m4[i][0] <= ctx.window_ctx.mode->width) &&
        (*m4[i][1] >= 0 && *m4[i][1] <= ctx.window_ctx.mode->height)) {
      return true;
    }
  }
  return false;
#endif
}
