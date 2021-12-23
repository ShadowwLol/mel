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

MEL_THREADED_FUNCTION test_function(void * args){
	/* Do something related to threading */
	//printf("Hello\n");
	return 0;
}
MEL_THREADED_FUNCTION test_function2(void * args){
	/* Do something related to threading */
	//printf("World\n");
	return 0;
}

int main(void){
	ctx = MEL_ctx_init("Shadowws Game", 1280, 720, MEL_TRUE);

	//printf("%d => max samples\n", MEL_get_max_aa_samples());

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

	char c = 0;

	/* Main loop */
	while (!glfwWindowShouldClose(ctx.window_ctx.window)){
		MEL_update_camera(camera);

		MEL_poll_events(&ctx);

		glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
		glClear(GL_COLOR_BUFFER_BIT);

		MEL_thread_create(threads[0], test_function, NULL);
		MEL_thread_create(threads[1], test_function2, NULL);
		c = !c;
		set_str(&ctx.title, (c == 0) ? "test1" : "test2");

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
		for (size_t i = 0; i < (sizeof(threads)/sizeof(threads[0])); ++i){
			MEL_thread_join(threads[i]);
		}
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

void input(){
	int8_t KEY_ESCAPE = MEL_get_key(GLFW_KEY_ESCAPE);
	int8_t KEY_F11 = MEL_get_key(GLFW_KEY_F11);

	if (KEY_ESCAPE == MEL_KEY_RELEASE){
		/* Close the game */
		glfwSetWindowShouldClose(ctx.window_ctx.window, GLFW_TRUE);
	}

	if (KEY_F11 == MEL_KEY_RELEASE){
		/* Fullscreen the game */
		MEL_toggle_fullscreen(ctx);
	}

	int8_t KEY_SPACE = MEL_get_key(GLFW_KEY_SPACE);
	int8_t KEY_E = MEL_get_key(GLFW_KEY_E);

	int8_t KEY_W = MEL_get_key(GLFW_KEY_W);
	int8_t KEY_A = MEL_get_key(GLFW_KEY_A);
	int8_t KEY_S = MEL_get_key(GLFW_KEY_S);
	int8_t KEY_D = MEL_get_key(GLFW_KEY_D);

	int8_t KEY_LEFT = MEL_get_key(GLFW_KEY_LEFT);
	int8_t KEY_RIGHT = MEL_get_key(GLFW_KEY_RIGHT);

	/* Testing */
	if (KEY_SPACE == MEL_KEY_RELEASE){
		smiley.rect.color[0] = !smiley.rect.color[0];
	}

	if (KEY_E == MEL_KEY_RELEASE){
		ctx.vsync = !ctx.vsync;
	}

	//printf("W => %d && S => %d\n", keyboard[GLFW_KEY_W], keyboard[GLFW_KEY_S]);
	if (KEY_W && !KEY_S){
		camera.pos[1] -= 800 * MEL_delta();
	}else if (KEY_S && !KEY_W){
		camera.pos[1] += 800 * MEL_delta();
	}else{
		// FIXME: MEL_KEY_NONE MUST BE 0 && GLFW_KEY_RELEASE must be > 0
		// if var == 1; !var = 0 and MEL_KEY_NONE = -1
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
	/* * * * * */
}
