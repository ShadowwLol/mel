#include "engine/inc/mel.h"
#include "engine/inc/common.h"
#include "engine/inc/camera.h"
#include "engine/inc/texture.h"
#include "engine/inc/threads.h"

mel_t ctx;
MEL_Renderer2D Rend;
MEL_Camera camera;
MEL_Camera default_camera;

MEL_Texture smiley;
MEL_Texture crate;
MEL_ColorRect bg;
MEL_ColorRect rectangle;

i32 main(i32 argc, char** argv) {
  if (xf ==
      mel_init(&ctx, "Shadowws Game", 1280, 720,
               MEL_FULLSCREEN | MEL_VSYNC)) {
    return xf;
  }

  Rend = MEL_Renderer2D_init(ctx);


  /* Textures && Rects  */
  rectangle = MEL_init_rect(&Rend);
  rectangle.size[0] = 200;
  rectangle.size[1] = 400;
  rectangle.color[0] = 0.8f;
  rectangle.color[1] = 0.0f;
  rectangle.color[2] = 0.2f;
  rectangle.rotation = 70;

  bg = MEL_init_rect(&Rend);
  bg.size[0] = ctx.width;
  bg.size[1] = ctx.height;
  bg.color[0] = 0.2f;
  bg.color[1] = 0.0f;
  bg.color[2] = 0.8;

  smiley =
    MEL_load_tex(&Rend, "resources/images/smiley.png", GL_RGBA,
                 GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  smiley.rect.pos[0] = 640.0f;
  smiley.rect.pos[1] = 360.0f;
  smiley.rect.color[3] = 0.5f;
  crate =
    MEL_load_tex(&Rend, "resources/images/container.jpg", GL_RGB,
                 GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  smiley.rect.pos[0] = 640.0f;

  crate.rect.size[0] /= 2;
  crate.rect.size[1] /= 2;

  MEL_Thread threads[2] = { 0 };

  MEL_init_camera(default_camera);
  MEL_init_camera(camera);

  /* Main loop */
  while (!glfwWindowShouldClose(ctx.window)) {
    MEL_update_camera(camera);
    MEL_poll_events(&ctx);
    MEL_set_clear_color(20, 20, 20, 255);

    MEL_clear(GL_COLOR_BUFFER_BIT);

    /* Drawing */
    MEL_TIMER_START();
    //TICK(RENDERING);
    MEL_render();
    //TOCK(RENDERING);
    MEL_TIMER_END();

    printf("FPS:[%d]\n", MEL_fps());

  }

  /* * * * * * */

  /* Terminating */
  MEL_destroy_image(smiley);
  MEL_destroy_image(crate);
  MEL_Renderer2D_destroy(&Rend);
  MEL_quit(&ctx);
  /* * * * * * * */

  return 0;
}

/*
        input() controls main input of the program
        example:
                MEL_KEY <var> = MEL_get_key(GLFW_KEY_...);
        you can do:
                if (<var> == MEL_KEY_NONE)
        or:
                if (!<var>)
        and you can do:
                if (<var> == MEL_KEY_PRESS)
                if (<var> == MEL_KEY_REPEAT)
                if (<var> == MEL_KEY_RELEASE)
        or:
                if (<var>)
*/
void input() {
  MEL_KEY KEY_ESCAPE = MEL_get_key(GLFW_KEY_ESCAPE);
  MEL_KEY KEY_F11 = MEL_get_key(GLFW_KEY_F11);

  if (KEY_ESCAPE == MEL_KEY_RELEASE) {
    /* Close the game */
    glfwSetWindowShouldClose(ctx.window, GLFW_TRUE);
  }

  if (KEY_F11 == MEL_KEY_RELEASE) {
    /* Fullscreen the game */
    if (ctx.cfg & MEL_FULLSCREEN) {
      ctx.cfg &= ~MEL_FULLSCREEN;
    } else {
      ctx.cfg |= MEL_FULLSCREEN;
    }
  }

  /* Testing */
  MEL_KEY KEY_W = MEL_get_key(GLFW_KEY_W);
  MEL_KEY KEY_A = MEL_get_key(GLFW_KEY_A);
  MEL_KEY KEY_S = MEL_get_key(GLFW_KEY_S);
  MEL_KEY KEY_D = MEL_get_key(GLFW_KEY_D);
  MEL_KEY KEY_SPACE = MEL_get_key(GLFW_KEY_SPACE);

  MEL_KEY KEY_LEFT = MEL_get_key(GLFW_KEY_LEFT);
  MEL_KEY KEY_RIGHT = MEL_get_key(GLFW_KEY_RIGHT);
  MEL_KEY KEY_UP = MEL_get_key(GLFW_KEY_UP);
  MEL_KEY KEY_DOWN = MEL_get_key(GLFW_KEY_DOWN);

  MEL_KEY KEY_L = MEL_get_key(GLFW_KEY_L);

  if (KEY_SPACE == MEL_KEY_RELEASE) {
    smiley.rect.color[0] = !smiley.rect.color[0];
    printf("FPS: [%d]\n", MEL_fps());
  }

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
    //smiley.rect.rotation += 80.0f * MEL_delta();
  } else if (KEY_LEFT && !KEY_RIGHT) {
    camera.rotation -= 80.0f * MEL_delta();
    //smiley.rect.rotation -= 80.0f * MEL_delta();
  }

  if (KEY_UP && !KEY_DOWN) {
    camera.scale[0] += 0.1f * MEL_delta();
    camera.scale[1] += 0.1f * MEL_delta();
  } else if (KEY_DOWN && !KEY_UP) {
    camera.scale[0] -= 0.1f * MEL_delta();
    camera.scale[1] -= 0.1f * MEL_delta();
  }

  if (KEY_L == MEL_KEY_RELEASE) {
    const char* t1 = "A Game!";
    const char* t2 = "Not a Game!";
    strcmp(ctx.title.buffer, t1) == 0 ? set_str(&ctx.title,
                                                t2) : set_str(&ctx.title, t1);
  }
  /* * * * * */
}

void MEL_render(void) {
  MEL_begin2D(&Rend);

  MEL_draw_rect(ctx, &Rend, &bg, default_camera);
  crate.rect.size[0] = 20;
  crate.rect.size[1] = 20;
  for (u32 row = 0; row < 100; ++row) {
    for (u32 column = 0; column < 100; ++column) {
      crate.rect.pos[0] = column * 20;
      crate.rect.pos[1] = row * 20;
      MEL_draw_tex(ctx, &Rend, &crate, camera);
    }
  }

  MEL_end2D(&ctx, &Rend);
}

void error_callback(i32 error, const char* description) {
  mlog(LOG_ERROR, description);
}

void joystick_callback(i32 jid, i32 event) {
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

void MEL_local_mouse_pos(mel_t ctx, f64 * x, f64 * y) {
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

void window_size_callback(GLFWwindow * window, i32 width, i32 height) {
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
