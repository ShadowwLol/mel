#ifndef _MEL_IMAGE_H
#define _MEL_IMAGE_H

#include "mel.h"
#include "renderer2d.h"
#include "shaders.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"

typedef struct {
  GLuint id;      /* Texture ID : [i]               */
  GLuint texture; /* Texture    : [i]               */
  vec4 src;       /* src[0:1] => pos ; src[2:3] = sz */
  vec4 dst;       /* dst[0:1] => pos ; dst[2:3] = sz */
  vec2 sz;
  vec4 col;
  f32 rot;

  mat4 mvp;
} texture_t;

texture_t mel_rect(MEL_Renderer2D * Renderer);
texture_t MEL_load_tex(MEL_Renderer2D *, GLchar *, GLenum, GLenum, GLenum);
void MEL_draw_rect(mel_t, MEL_Renderer2D *, texture_t *,
                  MEL_Camera);
void MEL_draw_tex(mel_t, MEL_Renderer2D *, texture_t *,
                  MEL_Camera);
void mel_free_texture(texture_t * texture);

#endif
