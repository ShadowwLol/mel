#include "inc/mel.h"	/* for gl defs	*/
#include "inc/IO.h"		/* for i/o		*/
#include "inc/def.h"		/* for configs	*/
#include "inc/common.h"	/* for MEL_ctx	*/
#include "inc/camera.h"	/* for cameras	*/
#include "inc/texture.h" /* for textures */
#include "inc/threads.h"	/* for threads	*/

MEL_ctx mctx;
MEL_Renderer2D Rend;
MEL_Camera camera;
MEL_Camera default_camera;

#define TESTING

#ifdef TESTING
MEL_Texture smiley;
MEL_Texture crate;
MEL_ColorRect bg;
MEL_ColorRect rectangle;
#endif

int main(void){
	mctx = MEL_ctx_init("Shadowws Game", 1280, 720, true);
	Rend = MEL_Renderer2D_init(mctx.window_ctx);

#ifdef TESTING

	/* Textures && Rects  */
	rectangle = MEL_init_rect(&Rend);
	rectangle.size[0] = 200;
	rectangle.size[1] = 400;
	rectangle.color[0] = 0.8f;
	rectangle.color[1] = 0.0f;
	rectangle.color[2] = 0.2f;
	rectangle.rotation = 70;

	bg = MEL_init_rect(&Rend);
	bg.size[0] = mctx.width;
	bg.size[1] = mctx.height;
	bg.color[0] = 0.2f;
	bg.color[1] = 0.0f;
	bg.color[2] = 0.8;

	smiley = MEL_load_tex(&Rend, "resources/images/smiley.png", GL_RGBA, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	smiley.rect.pos[0] = 640.0f;
	smiley.rect.pos[1] = 360.0f;
	smiley.rect.color[3] = 0.5f;
	crate = MEL_load_tex(&Rend, "resources/images/container.jpg", GL_RGB, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	smiley.rect.pos[0] = 640.0f;

	crate.rect.size[0] /= 2;
	crate.rect.size[1] /= 2;
#endif

	MEL_Thread threads[2] = {0};

	MEL_init_camera(default_camera);
	MEL_init_camera(camera);

	//mctx.vsync = false;

	/* Main loop */
	while (!glfwWindowShouldClose(mctx.window_ctx.window)){
		MEL_update_camera(camera);
		MEL_poll_events(&mctx);
		MEL_set_clear_color(20, 20, 20, 255);

		MEL_clear(GL_COLOR_BUFFER_BIT);

		/* Drawing */
		MEL_TIMER_START();
		//TICK(RENDERING);
		MEL_render();
		//TOCK(RENDERING);
		MEL_TIMER_END();

	}

	/* * * * * * */

	/* Terminating */
#ifdef TESTING
	MEL_destroy_image(smiley);
	MEL_destroy_image(crate);
#endif
	MEL_Renderer2D_destroy(&Rend);
	MEL_quit(&mctx);
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
		glfwSetWindowShouldClose(mctx.window_ctx.window, GLFW_TRUE);
	}

	if (KEY_F11 == MEL_KEY_RELEASE){
		/* Fullscreen the game */
		MEL_toggle_fullscreen(mctx);
	}

#ifdef TESTING
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

	if (KEY_SPACE == MEL_KEY_RELEASE){
		smiley.rect.color[0] = !smiley.rect.color[0];
		printf("FPS: [%d]\n", MEL_fps());
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
		camera.rotation += 80.0f * MEL_delta();
		//smiley.rect.rotation += 80.0f * MEL_delta();
	}else if (KEY_LEFT && !KEY_RIGHT){
		camera.rotation -= 80.0f * MEL_delta();
		//smiley.rect.rotation -= 80.0f * MEL_delta();
	}

	if (KEY_UP && !KEY_DOWN){
		camera.scale[0] += 0.1f * MEL_delta();
		camera.scale[1] += 0.1f * MEL_delta();
	}else if (KEY_DOWN && !KEY_UP){
		camera.scale[0] -= 0.1f * MEL_delta();
		camera.scale[1] -= 0.1f * MEL_delta();
	}

	if (KEY_L == MEL_KEY_RELEASE){
		const char * t1 = "A Game!";
		const char * t2 = "Not a Game!";
		strcmp(mctx.title.buffer, t1) == 0 ? set_str(&mctx.title, t2) : set_str(&mctx.title, t1);
	}
	/* * * * * */
#endif
}

void MEL_render(void){
	MEL_begin2D(&Rend);

#ifdef TESTING
	MEL_draw_rect(mctx, &Rend, &bg, default_camera);
	crate.rect.size[0] = 20;
	crate.rect.size[1] = 20;
    for (uint32_t row = 0; row < 100; ++row){
        for (uint32_t column = 0; column < 100; ++column){
            crate.rect.pos[0] = column * 20;
            crate.rect.pos[1] = row * 20;
			MEL_draw_tex(mctx, &Rend, &crate, camera);
        }
    }
#endif

	MEL_end2D(&Rend);
}
