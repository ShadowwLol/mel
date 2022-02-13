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
		int i;
		float id;
		static int field_len;
		static nk_size prog_value = 60;
		static int current_weapon = 0;
		static char field_buffer[64];
		static float pos;
		static const char *weapons[] = {"Fist","Pistol","Shotgun","Plasma","BFG"};
		const float step = (2*3.141592654f) / 32;

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

		nk_layout_row_dynamic(&nk.ctx, 20, 1);
		nk_label(&nk.ctx, "Ganda Label Bro", NK_TEXT_CENTERED);
		nk_checkbox_label(&nk.ctx, "working label with checkbox and a really long line of text", &check);
		nk_layout_row_dynamic(&nk.ctx, 30, 2);

		nk_check_label( &nk.ctx, "inactive", 0);
		nk_check_label( &nk.ctx, "active", 1);
		nk_option_label(&nk.ctx, "active", 1);
		nk_option_label(&nk.ctx, "inactive", 0);

		nk_label(&nk.ctx, "Smiley Stats", NK_TEXT_CENTERED);
		nk_layout_row_dynamic(&nk.ctx, 30, 1);

		nk_label(&nk.ctx, "R:", NK_TEXT_LEFT);
		nk_slider_float(&nk.ctx, 0, &R_slider, 1, 0.001);
		nk_label(&nk.ctx, "G:", NK_TEXT_LEFT);
		nk_slider_float(&nk.ctx, 0, &G_slider, 1, 0.001);
		nk_label(&nk.ctx, "B:", NK_TEXT_LEFT);
		nk_slider_float(&nk.ctx, 0, &B_slider, 1, 0.001);
		//smiley.rect.color[0] = R_slider;
		//smiley.rect.color[1] = G_slider;
		//smiley.rect.color[2] = B_slider;

		nk_layout_row_dynamic(&nk.ctx, 20, 1);
		nk_progress(&nk.ctx, &prog_value, 100, NK_MODIFIABLE);

		nk_layout_row_dynamic(&nk.ctx, 25, 1);
		nk_edit_string(&nk.ctx, NK_EDIT_FIELD, field_buffer, &field_len, 64, nk_filter_default);
		nk_property_float(&nk.ctx, "#X:", -1024.0f, &pos, 1024.0f, 1, 1);
		current_weapon = nk_combo(&nk.ctx, weapons, LEN(weapons), current_weapon, 25, nk_vec2(nk_widget_width(&nk.ctx),200));

		nk_label(&nk.ctx, "Sheeesh", NK_TEXT_LEFT);
		nk_layout_row_dynamic(&nk.ctx, 100, 1);
		if (nk_chart_begin_colored(&nk.ctx, NK_CHART_COLUMN, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
			nk_chart_add_slot_colored(&nk.ctx, NK_CHART_COLUMN, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
			nk_chart_add_slot_colored(&nk.ctx, NK_CHART_COLUMN, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, -1.0f, 1.0f);
			for (id = 0, i = 0; i < 32; ++i) {
				nk_chart_push_slot(&nk.ctx, (float)fabs(sin(id)), 0);
				nk_chart_push_slot(&nk.ctx, (float)cos(id), 1);
				nk_chart_push_slot(&nk.ctx, (float)sin(id), 2);
				id += step;
			}
		}
		nk_chart_end(&nk.ctx);

		nk_layout_row_dynamic(&nk.ctx, 250, 1);
		if (nk_group_begin(&nk.ctx, "Standard", NK_WINDOW_BORDER|NK_WINDOW_BORDER))
		{
			if (nk_tree_push(&nk.ctx, NK_TREE_NODE, "Window", NK_MAXIMIZED)) {
				static int selected[8];
				if (nk_tree_push(&nk.ctx, NK_TREE_NODE, "Next", NK_MAXIMIZED)) {
					nk_layout_row_dynamic(&nk.ctx, 20, 1);
					for (i = 0; i < 4; ++i)
						nk_selectable_label(&nk.ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_LEFT, &selected[i]);
					nk_tree_pop(&nk.ctx);
				}
				if (nk_tree_push(&nk.ctx, NK_TREE_NODE, "Previous", NK_MAXIMIZED)) {
					nk_layout_row_dynamic(&nk.ctx, 20, 1);
					for (i = 4; i < 8; ++i)
						nk_selectable_label(&nk.ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_LEFT, &selected[i]);
					nk_tree_pop(&nk.ctx);
				}
				nk_tree_pop(&nk.ctx);
			}
			nk_group_end(&nk.ctx);
		}
	}
	nk_end(&nk.ctx);
	/* * * * * */

	//printf("%lf secs elapsed\n", MEL_TIME_ELAPSED());
	//printf("%d\n", MEL_fps());
	//glViewport(mctx->window_mctx->padx, mctx->window_mctx->pady, display_width, display_height);
	//device_draw(&nk.device, &nk.ctx, width, height, scale, NK_ANTI_ALIASING_ON);
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
