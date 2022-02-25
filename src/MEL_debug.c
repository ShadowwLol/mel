#include "../include/MEL_debug.h"
#include "../include/gui.h"

#define DEBUG_W (100)
#define DEBUG_H (200)

static NK_CTX nk;

void MEL_init_debug(void){
	device_init(&nk.device);
	MEL_set_debug_font(NULL);

	/* GUI */
	NK_STYLE_WINDOW(	 nk.ctx);
	NK_STYLE_SCROLLBAR(  nk.ctx);
	NK_STYLE_CHECKBOX(	 nk.ctx);
	NK_STYLE_TOGGLE(	 nk.ctx);
	NK_STYLE_BUTTON(	 nk.ctx);
	NK_STYLE_TREE(		 nk.ctx);
	NK_STYLE_SELECTABLE( nk.ctx);
	NK_STYLE_SLIDER(	 nk.ctx);
	NK_STYLE_PROGRESSBAR(nk.ctx);
	NK_STYLE_COMBO(		 nk.ctx);
	NK_STYLE_PROPERTY(	 nk.ctx);
	NK_STYLE_CHART(		 nk.ctx);
}

static bool wireframe_mode = false;
static float R_slider = 1.0f;
static float G_slider = 1.0f;
static float B_slider = 1.0f;
static nk_bool check = nk_true;

void MEL_debug_menu(MEL_ctx * mctx){
	MEL_calculate_fps();
	if (wireframe_mode){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	/* nk_label(&mctx, "label", NK_TEXT_CENTERED/LEFT/RIGHT); */
    /* Input */
    double x, y;
    nk_input_begin(&nk.ctx);
	MEL_local_mouse_pos(*mctx, &x, &y);

    nk_input_motion(&nk.ctx, (int)x, (int)y);
    nk_input_button(&nk.ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(mctx->window_ctx.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    nk_input_button(&nk.ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(mctx->window_ctx.window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    nk_input_button(&nk.ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(mctx->window_ctx.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    nk_input_end(&nk.ctx);
	if (nk_begin(&nk.ctx, "Debug Menu", nk_rect(50, 50, 300, 400), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_TITLE)){
		nk_layout_row_static(&nk.ctx, 30, 120, 1);

		char fps[100] = "\0";
		snprintf(fps, 99, "FPS: [%d]", MEL_fps());
		nk_label(&nk.ctx, fps, NK_TEXT_LEFT);

		nk_layout_row_static(&nk.ctx, 30, 120, 1);
		if (nk_button_label(&nk.ctx, (wireframe_mode) ? "Wireframe: ON " : "Wireframe: OFF")){
			wireframe_mode = !wireframe_mode;
		}

		if (nk_button_label(&nk.ctx, (mctx->vsync) ? "VSync: ON " : "VSync: OFF")){
			mctx->vsync = !mctx->vsync;
		}

	}
	nk_end(&nk.ctx);
	/* * * * * */

	device_draw(&nk.device, &nk.ctx, DEBUG_W, DEBUG_H, ((struct nk_vec2){1,1}), NK_ANTI_ALIASING_OFF);

	if (wireframe_mode){
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
}

void MEL_end_debug(void){
	nk_font_atlas_clear(&nk.atlas);
	nk_free(&nk.ctx);
	NK_DEVICE_SHUTDOWN(nk.device);

}

void MEL_set_debug_font(const char * path){
	nk_font_atlas_init_default(&nk.atlas);
	nk_font_atlas_begin(&nk.atlas);

	if (path){
		nk.font =  nk_font_atlas_add_from_file(&nk.atlas, path, 13.0f, NULL);
	}else{
		nk.font = nk_font_atlas_add_default(&nk.atlas, 13.0f, NULL);
	}

	int32_t w, h;
	const void * image = nk_font_atlas_bake(&nk.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
	device_upload_atlas(&nk.device, image, w, h);
	nk_font_atlas_end(&nk.atlas, nk_handle_id((int)nk.device.font_tex), &nk.device.null);
	nk_init_default(&nk.ctx, &nk.font->handle);
}
