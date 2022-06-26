#ifndef _MEL_DEF_H
#define _MEL_DEF_H

#include "common.h"
#include "misc.h"

typedef struct {
  GLFWwindow* window;
  GLFWvidmode* mode;
  String title;                 /* Window title                                       */
  f64 initial_width, initial_height;  /* Window dimensions       => [widthxheight]          */
  f64 width, height;
  flag cfg;
  GLint MSAA;                   /* Antialiasing samples < MEL_get_max_aa_samples()    */
} mel_t;

static mel_t old = { NULL };

static GLint _MAX_MSAA;

/* flag definition */
#define MEL_VSYNC      (1)
#define MEL_FULLSCREEN (2)
#define MEL_RESIZABLE  (4)
#define MEL_CENTERED   (8)
#define MEL_FOCUSED    (16)
/* * * * * * * * * */

static GLint MEL_get_max_aa_samples();
static fn mel_init(mel_t * ctx, const char* title, f64 width,
                   f64 height, flag cfg);
static void update_cfg(mel_t * ctx);
static void MEL_quit(mel_t * ctx);

static void MEL_poll_events(mel_t * ctx);

/* Input */
static int16_t keyboard[512];

#define MEL_KEY_NONE    (0)
#define MEL_KEY_PRESS   (1)
#define MEL_KEY_REPEAT  (2)
#define MEL_KEY_RELEASE (3)

static void k_input(GLFWwindow *, int, int, int, int);
inline static void k_input(GLFWwindow * window, int key, int scancode,
                           int action, int mods) {
  if (action == GLFW_RELEASE) {
    action = MEL_KEY_RELEASE;
  }
  keyboard[key] = action;
}

static int8_t MEL_get_key(int16_t key);
inline static int8_t MEL_get_key(int16_t key) {
  return keyboard[key];
}

void input(void);
void MEL_render(void);
/* * * * */
void window_size_callback(GLFWwindow *, int, int);
void error_callback(int, const char* );
void joystick_callback(int, int);
void MEL_global_mouse_pos(mel_t ctx, double* x, double* y);
void MEL_local_mouse_pos(mel_t ctx, double* x, double* y);

inline static GLint MEL_get_max_aa_samples() {
  return _MAX_MSAA;
}

inline static fn mel_init(mel_t * ctx, const char* title, f64 width,
                          f64 height, flag cfg) {

  ctx->initial_width = width;
  ctx->initial_height = height;
  ctx->width = width;
  ctx->height = height;
  ctx->cfg = cfg;
  ctx->MSAA = 0;

  ctx->title = init_str(title);

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

  glGetIntegerv(GL_MAX_SAMPLES, &_MAX_MSAA);
  ctx->MSAA = _MAX_MSAA;

  GLint major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  mlog(LOG_INFORMATION, "OpenGL version: %d.%d", major, minor);
  /* * * * * * * * * * * * */

  /* Callbacks */
  glfwSetErrorCallback(error_callback);
  glfwSetInputMode(ctx->window, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSetKeyCallback(ctx->window, k_input);
  glfwSetFramebufferSizeCallback(ctx->window, window_size_callback);
  glfwSetWindowSizeLimits(ctx->window,
                          (ctx->mode->width / 2),
                          (ctx->mode->height / 2),
                          ctx->mode->width, ctx->mode->height);
  /* * * * * * */

  /* Initializing keyboard */
  for (uint16_t i = 0; i < (sizeof(keyboard) / sizeof(keyboard[0])); ++i) {
    keyboard[i] = MEL_KEY_NONE;
  }
  /* * * * * * * * * * * * */

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
    window_size_callback(ctx->window, old.width, old.height);
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
      if (ctx->MSAA <= _MAX_MSAA) {
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

inline static void MEL_poll_events(mel_t * ctx) {
  update_cfg(ctx);
  glfwPollEvents();
  input();
  for (uint16_t i = 0; i < (sizeof(keyboard) / sizeof(keyboard[0])); ++i) {
    if (keyboard[i] == MEL_KEY_RELEASE) {
      keyboard[i] = MEL_KEY_NONE;
    }
  }
  MEL_calculate_delta();
  MEL_calculate_fps();
}

#endif
