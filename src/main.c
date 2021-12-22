#include "../include/MEL_opengl.h" /* for gl defs */
#include "../include/MEL_IO.h"     /* for i/o     */
#include "../include/MEL_def.h"    /* for configs */
#include "../include/MEL_common.h"
#include "../include/MEL_shader.h" /* for shaders */
#include "../include/MEL_Camera.h" /* for cameras */
#include "../include/MEL_logs.h"   /* for logging */
#include "../include/MEL_Texture.h"  /* for images  */
#include "../include/MEL_misc.h"   /* for misc    */
#include "../include/MEL_thread.h"

MEL_ctx ctx;
MEL_Texture smiley;
MEL_Camera camera;
MEL_Renderer2D Rend;

MEL_THREADED_FUNCTION test_function(void * args){
	/* Do something related to threading */
	printf("Hello\n");
	return 0;
}
MEL_THREADED_FUNCTION test_function2(void * args){
	/* Do something related to threading */
	printf("World\n");
	return 0;
}

int main(void){
	ctx = MEL_ctx_init("Shadowws Game", 1280, 720, MEL_TRUE);
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
		update_window(ctx);
		MEL_update_camera(camera);
		glfwPollEvents();
		MEL_calculate_delta();
		MEL_calculate_fps();
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
		printf("%d\n", MEL_fps());
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

/* FIXME: Become keycallback with hashmap for every key */
void input(GLFWwindow* window, int key, int scancode, int action, int mods){
		switch(action){
		case GLFW_RELEASE:
			switch(key){
				case GLFW_KEY_ESCAPE:
					/* Close the game */
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
				case GLFW_KEY_F11:
					/* Fullscreen the game */
					MEL_toggle_fullscreen(ctx);
					break;
				case GLFW_KEY_SPACE:
					if (smiley.rect.color[0] == 1.0f){
						smiley.rect.color[0] = 0.0f;
					}else{
						smiley.rect.color[0] = 1.0;
					}
					break;
				case GLFW_KEY_E:
					ctx.vsync = !ctx.vsync;
					break;
				default:
					break;
			}
			break;
		case GLFW_REPEAT:
			switch(key){
				case GLFW_KEY_W:
					camera.pos[1] -= 800 * MEL_delta();
					//smiley.rect.pos[1] -= 10;
					break;
				case GLFW_KEY_A:
					camera.pos[0] -= 800 * MEL_delta();
					//smiley.rect.pos[0] -= 10;
					break;
				case GLFW_KEY_S:
					camera.pos[1] += 800 * MEL_delta();
					//smiley.rect.pos[1] += 10;
					break;
				case GLFW_KEY_D:
					camera.pos[0] += 800 * MEL_delta();
					//smiley.rect.pos[0] += 10;
					break;
				case GLFW_KEY_RIGHT:
					smiley.rect.rotation += 80.0f * MEL_delta();
					break;
				case GLFW_KEY_LEFT:
					smiley.rect.rotation -= 80.0f * MEL_delta();
					break;
				default:
					break;
			}
		default:
			break;
	}
	return;
}
