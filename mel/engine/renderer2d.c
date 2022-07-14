#include "inc/renderer2d.h"
#include "inc/texture.h"

MEL_Renderer2D MEL_Renderer2D_init(mel_t ctx) {
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
  glm_ortho(0.0f, (float) ctx.mode->width, (float) ctx.mode->height, 0.0f,
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

void MEL_begin2D(MEL_Renderer2D * Renderer) {
  glBindVertexArray(Renderer->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, Renderer->VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer->EBO);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(Renderer->shader);
}

void MEL_end2D(mel_t * ctx, MEL_Renderer2D * Renderer) {
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
  glfwSwapBuffers(ctx->window);
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
