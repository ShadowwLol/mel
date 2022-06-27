#ifndef _MEL_H
#define _MEL_H

#include "common.h"
#include "input.h"
#include "misc.h"

typedef struct {
  GLFWwindow* window;
  GLFWvidmode* mode;
  String title;
  f64 initial_width, initial_height;
  f64 width, height;
  GLint MSAA, MAX_MSAA;
  flag cfg;

  void (*init)(void);
  void (*quit)(void);

  void (*update)(void);
  void (*resize)(GLFWwindow *, i32, i32);

  void (*input)(void);
  void (*joystick)(i32, i32);
} mel_t;

static mel_t old = { NULL };

/* flag definition */
#define MEL_VSYNC      (1)
#define MEL_FULLSCREEN (2)
#define MEL_RESIZABLE  (4)
#define MEL_CENTERED   (8)
#define MEL_FOCUSED    (16)
/* * * * * * * * * */

/* mel (de)initialization  */
static fn mel_init(mel_t * ctx, const char* title,
                   f64 width, f64 height,
                   void (*init)(void), void(*quit)(void),
                   void(*update)(void), void(*resize)(GLFWwindow *, i32,
                                                      i32),
                   void(*input)(void), void(*joystick)(i32, i32), u8 cfg);
static void mel_free(mel_t * ctx);
/* * * * * * * * * * * * * */

static void mel_poll(mel_t * ctx);

inline static void error_callback(i32 error, const char* description) {
  mlog(LOG_ERROR, description);
}

inline static fn mel_init(mel_t * ctx, const char* title,
                          f64 width, f64 height,
                          void (*init)(void),
                          void (*quit)(void),
                          void (*update)(void),
                          void (*resize)(GLFWwindow *, i32, i32),
                          void (*input)(void),
                          void (*joystick)(i32, i32), flag cfg) {

  if(!ctx || !title || !width || !height || !init || !quit || !update
     || !resize || !input || !joystick) {
    mlog(LOG_ERROR, "Failed initializing MEL");
    return xf;
  }
  mlog(LOG_SUCCESS, "Successfully initialized MEL");

  ctx->title = init_str(title);
  ctx->initial_width = width;
  ctx->initial_height = height;
  ctx->width = width;
  ctx->height = height;
  ctx->MSAA = 0;
  ctx->cfg = cfg;
  ctx->init = init;
  ctx->quit = quit;
  ctx->update = update;
  ctx->resize = resize;
  ctx->input = input;
  ctx->joystick = joystick;

  /* Initializing the library  */
  if (!glfwInit()) {
    mlog(LOG_ERROR, "Failed initializing GLFW");
    return xf;
  } else {
    mlog(LOG_SUCCESS, "Successfully initialized GLFW");
  }
  /* * * * * * * * * * * * * * */

  /* Window hints  */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, 1);
  /* * * * * * * * * * * */

  /* Creating the window */
  ctx->mode = (GLFWvidmode *) glfwGetVideoMode(glfwGetPrimaryMonitor());
  ctx->window =
    glfwCreateWindow(ctx->width, ctx->height, ctx->title.buffer,
                     (cfg & MEL_FULLSCREEN) ? glfwGetPrimaryMonitor() : NULL,
                     NULL);

  if (!ctx->window) {
    mlog(LOG_ERROR, "Failed creating window");
    glfwTerminate();
    return xf;
  } else {
    mlog(LOG_SUCCESS, "Successfully created window");
  }
  /* * * * * * * * * * * */

  /* Make the window's context current */
  glfwMakeContextCurrent(ctx->window);
  /* * * * * * * * * * * * * * * * * * */

  /* OPENGL configuration  */
  gladLoadGL();
  glViewport(0, 0, width, height);

  glGetIntegerv(GL_MAX_SAMPLES, &ctx->MAX_MSAA);
  ctx->MSAA = ctx->MAX_MSAA;

  GLint major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  mlog(LOG_INFORMATION, "OpenGL version: %d.%d", major, minor);
  /* * * * * * * * * * * * */

  /* Callbacks */
  glfwSetErrorCallback(error_callback);
  glfwSetInputMode(ctx->window, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSetKeyCallback(ctx->window, _input);
  glfwSetFramebufferSizeCallback(ctx->window, ctx->resize);
  glfwSetWindowSizeLimits(ctx->window,
                          (ctx->mode->width / 2),
                          (ctx->mode->height / 2),
                          ctx->mode->width, ctx->mode->height);
  /* * * * * * */

  init_keyboard();
  ctx->init();

  while (!glfwWindowShouldClose(ctx->window)) {
    mel_poll(ctx);
    ctx->update();
  }

  ctx->quit();

  return xs;
}

inline static void update_cfg(mel_t * ctx) {

  /* Update window title */
  if (!old.title.buffer || strncmp(ctx->title.buffer, old.title.buffer,
                                   ((ctx->title.length >
                                     old.title.length) ? ctx->title.
                                    length : old.title.length)) != 0) {
    set_str(&old.title, ctx->title.buffer);
    glfwSetWindowTitle(ctx->window, old.title.buffer);
  }
  /* * * * * * * * * * * */

  /* Update window size  */
  if (ctx->width != old.width || ctx->height != old.height) {
    old.width = ctx->width;
    old.height = ctx->height;
    ctx->resize(ctx->window, old.width, old.height);
  }
  /* * * * * * * * * * * */

  /* Updating window configuration */
  if (ctx->cfg != old.cfg) {

    /* Updating MEL_VSYNC  */
    if ((ctx->cfg & MEL_VSYNC) != (old.cfg & MEL_VSYNC)) {
      glfwSwapInterval((ctx->cfg & MEL_VSYNC));
    }
    /* * * * * * * * * * * */

    /* Updating MEL_FULLSCREEN && MEL_CENTERED */
    if ((ctx->cfg & MEL_FULLSCREEN) != (old.cfg & MEL_FULLSCREEN)) {
      glfwSetWindowMonitor(ctx->window,
                           (ctx->cfg & MEL_FULLSCREEN) ?
                           glfwGetPrimaryMonitor() : NULL,
                           (ctx->cfg & MEL_CENTERED)
                           ? (((float) ctx->mode->width / 2.0f) -
                              (ctx->initial_width / 2.0f)) : 0.0,
                           (ctx->cfg & MEL_CENTERED)
                           ? (((float) ctx->mode->height / 2.0f) -
                              (ctx->initial_height / 2.0f)) : 0.0,
                           ctx->initial_width, ctx->initial_height,
                           ctx->mode->refreshRate);
    }
    /* * * * * * * * * * * * * * * * * * * * * */

    /* Updating MEL_RESIZABLE  */
    if ((ctx->cfg & MEL_RESIZABLE) != (old.cfg & MEL_RESIZABLE)) {
      glfwWindowHint(GLFW_RESIZABLE,
                     (ctx->cfg & MEL_RESIZABLE) ? GLFW_TRUE : GLFW_FALSE);
    }
    /* * * * * * * * * * * * * */

    /* Updating MEL_FOCUSED  */
    if (ctx->cfg & MEL_FOCUSED && !(old.cfg & MEL_FOCUSED)) {
      glfwFocusWindow(ctx->window);
    }
    /* * * * * * * * * * * * */

    /* Updating MSAA */
    if (ctx->MSAA != old.MSAA) {
      if (ctx->MSAA <= ctx->MAX_MSAA) {
        old.MSAA = ctx->MSAA;
        glfwWindowHint(GLFW_SAMPLES, old.MSAA);
      } else {
        ctx->MSAA = old.MSAA;
      }
    }
    /* * * * * * * * */

    old.cfg = ctx->cfg;
  }
  /* * * * * * * * * * * * * * * * */
}

inline static void MEL_quit(mel_t * ctx) {
  remove_str(&ctx->title);
  remove_str(&old.title);
  glfwDestroyWindow(ctx->window);
  glfwTerminate();
}

inline static void mel_poll(mel_t * ctx) {
  update_cfg(ctx);
  glfwPollEvents();
  ctx->input();
  reset_keyboard();
  MEL_calculate_delta();
  MEL_calculate_fps();
}

#endif /* _MEL_H */
