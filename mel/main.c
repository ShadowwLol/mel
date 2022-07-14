#include "engine/inc/mel.h"
#include "engine/inc/common.h"
#include "engine/inc/camera.h"
#include "engine/inc/texture.h"
#include "engine/inc/threads.h"

static void init(void);
static void quit(void);

static void update(void);
static void resize(GLFWwindow *, i32, i32);

static void input(void);
static void joystick(i32, i32);

mel_t ctx;
MEL_Renderer2D Rend;
MEL_Camera camera;
MEL_Camera default_camera;

texture_t atlas;
texture_t bg;

/*! here you can (de)initialize the mel context used
   and run non-OpenGL code before the application  */
i32 main(i32 argc, char** argv) {
  return mel_init(&ctx, "Shadowws Game", 1280, 720, init, quit, update,
                  resize, input, joystick, MEL_FULLSCREEN | MEL_VSYNC);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * */

/*! here you can write OpenGL code
   that runs before the whole application  */
static void init(void) {
  Rend = MEL_Renderer2D_init(ctx);

  /* Textures && Rects  */
  bg = mel_rect(&Rend);
  bg.dst[2] = ctx.width;
  bg.dst[3] = ctx.height;
  bg.sz[0] = ctx.width;
  bg.sz[1] = ctx.height;
  bg.col[0] = 0.2f;
  bg.col[1] = 0.0f;
  bg.col[2] = 0.8;

  atlas =
    MEL_load_tex(&Rend, "resources/images/atlas.png", GL_RGBA,
                 GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

  atlas.src[0]    = 0.0f;
  atlas.src[1]    = 0.0f;
  atlas.src[2]   /= 4; // 4 = hindex
  atlas.src[3]   /= 4; // 4  = vindex
  atlas.dst[2] /= 2;
  atlas.dst[3] /= 2;

  MEL_init_camera(default_camera);
  MEL_init_camera(camera);
}

/* * * * * * * * * * * * * * * * * * * * * */

/*! here you can write OpenGL code
   that runs after the whole application  */
static void quit(void) {
  mel_free_texture(&atlas);
  MEL_Renderer2D_destroy(&Rend);
  MEL_quit(&ctx);
}

/* * * * * * * * * * * * * * * * * * * * * */

/* here you can write OpenGL code
   that runs every frame during the whole application  */
static void update(void) {
  MEL_update_camera(camera);

  glClearColor(20 / 255, 20 / 255, 20 / 255, 255 / 255);
  glClear(GL_COLOR_BUFFER_BIT);

  MEL_begin2D(&Rend);

  MEL_draw_rect(ctx, &Rend, &bg, default_camera);
  //crate.dest.size[0] = 20;
  //crate.dest.size[1] = 20;

//  /* draw 10,000 entities */
//  for (u32 row = 0; row < 100; ++row) {
//    for (u32 column = 0; column < 100; ++column) {
//      crate.dest.pos[0] = column * 20;
//      crate.dest.pos[1] = row * 20;
//      MEL_draw_tex(ctx, &Rend, &crate, camera);
//    }
//  }
//
  MEL_draw_tex(ctx, &Rend, &atlas, camera);
  MEL_end2D(&ctx, &Rend);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* here you can write OpenGL code
   that runs every time a resize event is triggered  */
static void resize(GLFWwindow * window, i32 width, i32 height) {
  ctx.width = width;
  ctx.height = height;

  f32 t_w = width;
  f32 t_h = (width / (ctx.initial_width / ctx.initial_height));
  if (t_h > height) {
    t_h = height;
    t_w = (t_h * (ctx.initial_width / ctx.initial_height));
  }

  glViewport(((width - t_w) / 2.0f), ((height - t_h) / 2.0f), t_w, t_h);
  glm_ortho(0, ctx.initial_width, ctx.initial_height, 0, -1.0f, 1.0f,
            Rend.projection);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * */

/* here you can write OpenGL code
   and check for any kind of input events every frame  */
static void input(void) {
  i8 KEY_ESCAPE = mel_key(GLFW_KEY_ESCAPE);
  if (KEY_ESCAPE == MEL_RELEASE) {
    /* quit */
    glfwSetWindowShouldClose(ctx.window, GLFW_TRUE);
  }

  i8 KEY_F11 = mel_key(GLFW_KEY_F11);
  if (KEY_F11 == MEL_RELEASE) {
    /* toggle fullscreen */
    (ctx.cfg & MEL_FULLSCREEN) ? (ctx.cfg &= ~MEL_FULLSCREEN) : (ctx.cfg |=
                                                                 MEL_FULLSCREEN);
  }

  /* Testing */
  i8 KEY_W = mel_key(GLFW_KEY_W);
  i8 KEY_A = mel_key(GLFW_KEY_A);
  i8 KEY_S = mel_key(GLFW_KEY_S);
  i8 KEY_D = mel_key(GLFW_KEY_D);

  i8 KEY_LEFT = mel_key(GLFW_KEY_LEFT);
  i8 KEY_RIGHT = mel_key(GLFW_KEY_RIGHT);
  i8 KEY_UP = mel_key(GLFW_KEY_UP);
  i8 KEY_DOWN = mel_key(GLFW_KEY_DOWN);

  i8 KEY_L = mel_key(GLFW_KEY_L);
  i8 KEY_SPACE = mel_key(GLFW_KEY_SPACE);

  if (KEY_W && !KEY_S) {
    camera.pos[1] -= 800 * MEL_delta();
  } else if (KEY_S && !KEY_W) {
    camera.pos[1] += 800 * MEL_delta();
  }

  if (KEY_A && !KEY_D) {
    camera.pos[0] -= 800 * MEL_delta();
  } else if (KEY_D && !KEY_A) {
    camera.pos[0] += 800 * MEL_delta();
  }

  if (KEY_RIGHT && !KEY_LEFT) {
    camera.rotation += 80.0f * MEL_delta();
  } else if (KEY_LEFT && !KEY_RIGHT) {
    camera.rotation -= 80.0f * MEL_delta();
  }

  if (KEY_UP && !KEY_DOWN) {
    camera.scale[0] += 0.4f * MEL_delta();
    camera.scale[1] += 0.4f * MEL_delta();
  } else if (KEY_DOWN && !KEY_UP) {
    camera.scale[0] -= 0.4f * MEL_delta();
    camera.scale[1] -= 0.4f * MEL_delta();
  }

  if (KEY_SPACE == MEL_RELEASE){
    atlas.src[0] += atlas.sz[0] / 4;
    if (atlas.src[0] >= atlas.sz[0]){
      atlas.src[0] = 0.0f;
    }
  }

  /* * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* here you can write OpenGL code
   that runs every time a joystick event is triggered  */
static void joystick(i32 jid, i32 event) {
  switch (event) {
    case GLFW_CONNECTED:
      mlog(LOG_INFORMATION, "Controller [%d]{%s} connected!", jid,
           glfwGetGamepadName(jid));
      break;
    case GLFW_DISCONNECTED:
      mlog(LOG_INFORMATION, "Controller [%d]{%s} disconnected!", jid,
           glfwGetGamepadName(jid));
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mel_local_mouse_pos(mel_t ctx, f64 * x, f64 * y) {
  glfwGetCursorPos(ctx.window, x, y);
  vec2 scale = { (ctx.width / ctx.initial_width),
    (ctx.height / ctx.initial_height)
  };
  vec2 real = { ctx.initial_width * scale[0], ctx.initial_height * scale[1] };
  vec2 r = { ((ctx.width / 2) - (real[0] / 2)),
    ((ctx.height / 2) - (real[1] / 2))
  };
  vec2 adjustedMouse = { *x - r[0], *y - r[1] };

  *x = (f32) adjustedMouse[0] / (f32) real[0] * (f32) ctx.initial_width;
  *y = (f32) adjustedMouse[1] / (f32) real[1] * (f32) ctx.initial_height;
}
