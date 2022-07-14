#include "inc/mel.h"
#include "inc/logs.h"
#include "inc/texture.h"

texture_t MEL_load_tex(MEL_Renderer2D * Renderer, GLchar * path,
                         GLenum channels, GLenum min_filter,
                         GLenum max_filter) {
  stbi_set_flip_vertically_on_load(true);
  GLint w, h, c;
  stbi_uc* data = stbi_load(path, &w, &h, &c, 0);
  texture_t img = {
    .id =
      (Renderer->tex_count < Renderer->max_tex) ? ++Renderer->tex_count : 1,

    .src[0] = 0.0f,
    .src[1] = 0.0f,
    .src[2] = w,
    .src[3] = h,

    .dst[0] = 0.0f,
    .dst[1] = 0.0f,
    .dst[2] = w,
    .dst[3] = h,

    .sz[0] = w,
    .sz[1] = h,

    .col[0] = 1.0f,
    .col[1] = 1.0f,
    .col[2] = 1.0f,
    .col[3] = 1.0f,

    .rot    = 0.0f,

    .mvp = GLM_MAT4_IDENTITY_INIT,
  };

  if (Renderer->tex_count >= Renderer->max_tex) {
    Renderer->tex_count = 1;
  }

  /* glActiveTexture(GL_TEXTURE0+img.id); <= Only when batching */
  glGenTextures(1, &img.texture);
  glBindTexture(GL_TEXTURE_2D, img.texture);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (vec4) {
                   0.0f, 0.0f, 0.0f, 1.0f}
  );

  if (data) {
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, channels, img.dst[2],
                 img.dst[3], 0, channels, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    mlog(LOG_SUCCESS, "Successfully loaded image: {%s}", path);
  } else {
    mlog(LOG_ERROR, "Failed loading image: {%s}", path);
  }
  stbi_image_free(data);

  return img;
}

static void MEL_send_tex(MEL_Renderer2D * Renderer, texture_t texture) {
  vec4 coords, result = GLM_VEC4_ZERO_INIT;
  coords[2] = 0.0f;
  coords[3] = 1.0f;

  /* top left */
  coords[0] = texture.dst[0];
  coords[1] = texture.dst[1] + texture.sz[1];
  glm_mat4_mulv(texture.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 0] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 1] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 2] =
    texture.col[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 3] =
    texture.col[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 4] =
    texture.col[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 5] =
    texture.col[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 6] = ((texture.src[0]) / texture.sz[0]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 7] = ((texture.src[1]) / texture.sz[1]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 8] = texture.id;
  /* * * * * * */

  /* bottom left  */
  coords[0] = texture.dst[0];
  coords[1] = texture.dst[1];
  glm_mat4_mulv(texture.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 9] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 10] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 11] =
    texture.col[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 12] =
    texture.col[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 13] =
    texture.col[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 14] =
    texture.col[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 15] = ((texture.src[0]) / texture.sz[0]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 16] = ((texture.src[1] + texture.src[3]) / texture.sz[1]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 17] = texture.id;
  /* * * * * * * */

  /* bottom right  */
  coords[0] = texture.dst[0] + texture.sz[0];
  coords[1] = texture.dst[1];
  glm_mat4_mulv(texture.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 18] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 19] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 20] =
    texture.col[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 21] =
    texture.col[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 22] =
    texture.col[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 23] =
    texture.col[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 24] = ((texture.src[0] + texture.src[2]) / texture.sz[0]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 25] = ((texture.src[1] + texture.src[3]) / texture.sz[1]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 26] = texture.id;
  /* * * * * * * * */

  /* top right */
  coords[0] = texture.dst[0] + texture.sz[0];
  coords[1] = texture.dst[1] + texture.sz[1];
  glm_mat4_mulv(texture.mvp, coords, result);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 27] = result[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 28] = result[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 29] =
    texture.col[0];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 30] =
    texture.col[1];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 31] =
    texture.col[2];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 32] =
    texture.col[3];
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 33] = ((texture.src[0] + texture.src[2]) / texture.sz[0]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 34] = ((texture.src[1]) / texture.sz[1]);
  Renderer->vertices[(VERTEX_COUNT * Renderer->ID) + 35] = texture.id;
  /* * * * * * */

  ++Renderer->ID;
}

void MEL_draw_tex(mel_t ctx, MEL_Renderer2D * Renderer, texture_t * texture,
                  MEL_Camera Camera) {
  /* TODO: is_visible(texture) function */
  if (1) {
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

    MEL_send_tex(Renderer, *texture);
    glm_mat4_mul(Renderer->projection, Camera.view, texture->mvp);
    glm_rotate_at(texture->mvp, (vec3) {
                  (float) (texture->dst[0] + texture->dst[2] / 2.0f),
                  (float) (texture->dst[1] + texture->dst[3] / 2.0f),
                  0.0f}, glm_rad(texture->rot), (vec3) {
                  0.0f, 0.0f, 1.0f});
    /* https://stackoverflow.com/a/28360579/16946028 */
    /* glActiveTexture(GL_TEXTURE0+Img.id); <= Only when batching */
    glActiveTexture(GL_TEXTURE0 + texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

  }
}

void mel_free_texture(texture_t * texture){
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texture->texture);
}

texture_t mel_rect(MEL_Renderer2D * Renderer) {
  texture_t rect = {
    .mvp = GLM_MAT4_IDENTITY_INIT,
    .id = 0,
    .dst[0] = 0.0f,
    .dst[1] = 0.0f,
    .dst[2] = 1.0f,
    .dst[3] = 1.0f,
    .sz[0]  = 1,
    .sz[1]  = 1,
    .col[0] = 1.0f,
    .col[1] = 1.0f,
    .col[2] = 1.0f,
    .col[3] = 1.0f,
    .rot    = 0.0f,
  };
  return rect;
}

void MEL_draw_rect(mel_t ctx, MEL_Renderer2D * Renderer,
                   texture_t * rect, MEL_Camera Camera) {
  /* TODO: is_visible(texture) function */
  if (1) {
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

    MEL_send_tex(Renderer, *rect);
    glm_mat4_mul(Renderer->projection, Camera.view, rect->mvp);
    glm_rotate_at(rect->mvp, (vec3) {
                  (float) (rect->dst[0] + rect->dst[0] / 2.0f),
                  (float) (rect->dst[1] + rect->dst[1] / 2.0f), 0.0f},
                  glm_rad(rect->rot), (vec3) {
                  0.0f, 0.0f, 1.0f});
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *Renderer->default_texture);
  }
}
