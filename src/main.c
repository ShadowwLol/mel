#include "../include/MEL_opengl.h"  /* for gl defs  */
#include "../include/MEL_IO.h"      /* for i/o      */
#include "../include/MEL_def.h"     /* for configs  */
#include "../include/MEL_common.h"  /* for MEL_ctx  */
#include "../include/MEL_Camera.h"  /* for cameras  */
#include "../include/MEL_Texture.h" /* for textures */
#include "../include/MEL_thread.h"  /* for threads  */

MEL_ctx ctx;
MEL_Texture smiley;
MEL_Camera camera;
MEL_Renderer2D Rend;

int main(void){
	ctx = MEL_ctx_init("Shadowws Game", 1280, 720, true);
	Rend = MEL_Renderer2D_init(ctx.window_ctx);

	/* Textures && Rects  */
	MEL_ColorRect rectangle = MEL_init_rect(&Rend);
	rectangle.size[0] = 200;
	rectangle.size[1] = 400;
	rectangle.color[0] = 0.8f;
	rectangle.color[1] = 0.0f;
	rectangle.color[2] = 0.2f;
	rectangle.rotation = 70;

	MEL_ColorRect bg = MEL_init_rect(&Rend);
	bg.size[0] = ctx.width;
	bg.size[1] = ctx.height;
	bg.color[0] = 0.2f;
	bg.color[1] = 0.0f;
	bg.color[2] = 0.8;

	smiley = MEL_load_image(&Rend, "resources/images/smiley.png", GL_RGBA, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	smiley.rect.pos[0] = 640.0f;
	smiley.rect.pos[1] = 360.0f;
	smiley.rect.color[3] = 0.5f;
	MEL_Texture crate = MEL_load_image(&Rend, "resources/images/container.jpg", GL_RGB, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	smiley.rect.pos[0] = 640.0f;

	crate.rect.size[0] /= 2;
	crate.rect.size[1] /= 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MEL_Thread threads[2] = {0};

	MEL_Camera default_camera;
	MEL_init_camera(default_camera);
	MEL_init_camera(camera);

	/* Main loop */
	while (!glfwWindowShouldClose(ctx.window_ctx.window)){
		MEL_update_camera(camera);

		MEL_poll_events(&ctx);

        MEL_set_clear_color(20, 20, 20, 255);
        MEL_clear(GL_COLOR_BUFFER_BIT);

		/* Drawing */
		MEL_TIMER_START();

		MEL_begin_rendering2D(Rend);

		MEL_draw_rect(ctx.window_ctx, Rend, bg, default_camera);
		MEL_update_image(ctx.window_ctx, Rend, crate,  camera, MEL_IMAGE_STATIC);
		MEL_update_image(ctx.window_ctx, Rend, smiley, camera, MEL_IMAGE_DYNAMIC);
		MEL_draw_rect(ctx.window_ctx, Rend, rectangle, camera);

		MEL_end_rendering2D();

		MEL_TIMER_END();
		/* * * * * */

		//printf("%lf secs elapsed\n", MEL_TIME_ELAPSED());
		//printf("%d\n", MEL_fps());
		glfwSwapBuffers(ctx.window_ctx.window);
	}

	/* * * * * * */

	/* Terminating */
	MEL_destroy_image(smiley);
	MEL_destroy_image(crate);
	MEL_Renderer2D_destroy(Rend);
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
void input(){
	MEL_KEY KEY_ESCAPE = MEL_get_key(GLFW_KEY_ESCAPE);
	MEL_KEY KEY_F11 = MEL_get_key(GLFW_KEY_F11);

	if (KEY_ESCAPE == MEL_KEY_RELEASE){
		/* Close the game */
		glfwSetWindowShouldClose(ctx.window_ctx.window, GLFW_TRUE);
	}

	if (KEY_F11 == MEL_KEY_RELEASE){
		/* Fullscreen the game */
		MEL_toggle_fullscreen(ctx);
	}

	MEL_KEY KEY_SPACE = MEL_get_key(GLFW_KEY_SPACE);
	MEL_KEY KEY_E = MEL_get_key(GLFW_KEY_E);

	MEL_KEY KEY_W = MEL_get_key(GLFW_KEY_W);
	MEL_KEY KEY_A = MEL_get_key(GLFW_KEY_A);
	MEL_KEY KEY_S = MEL_get_key(GLFW_KEY_S);
	MEL_KEY KEY_D = MEL_get_key(GLFW_KEY_D);

	MEL_KEY KEY_LEFT = MEL_get_key(GLFW_KEY_LEFT);
	MEL_KEY KEY_RIGHT = MEL_get_key(GLFW_KEY_RIGHT);

    MEL_KEY KEY_L = MEL_get_key(GLFW_KEY_L);

	/* Testing */
	if (KEY_SPACE == MEL_KEY_RELEASE){
		smiley.rect.color[0] = !smiley.rect.color[0];
	}

	if (KEY_E == MEL_KEY_RELEASE){
		ctx.vsync = !ctx.vsync;
	}

	if (KEY_W && !KEY_S){
		camera.pos[1] -= 800 * MEL_delta();
	}else if (KEY_S && !KEY_W){
		camera.pos[1] += 800 * MEL_delta();
	}

	if (KEY_A && !KEY_D){
		camera.pos[0] -= 800 * MEL_delta();
	}else if (KEY_D && !KEY_A){
		camera.pos[0] += 800 * MEL_delta();
	}

	if (KEY_RIGHT && !KEY_LEFT){
		smiley.rect.rotation += 80.0f * MEL_delta();
	}else if (KEY_LEFT && !KEY_RIGHT){
		smiley.rect.rotation -= 80.0f * MEL_delta();
	}

    if (KEY_L == MEL_KEY_RELEASE){
        const char * t1 = "A Game!";
        const char * t2 = "Not a Game!";
        strcmp(ctx.title.buffer, t1) == 0 ? set_str(&ctx.title, t2) : set_str(&ctx.title, t1);

    }
	/* * * * * */
}
