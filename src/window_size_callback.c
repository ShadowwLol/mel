#include "inc/mel.h"
#include "inc/def.h"
#include "inc/renderer2d.h"

extern MEL_ctx mctx;
extern MEL_Renderer2D Rend;

/**
 * @brief [
 *         Window resize callback:
 *         Maintains viewport aspect ratio and allows window resizing
 *        ]
 *
 * @param window [GLFW window]
 * @param width  [The new width]
 * @param height [The new height]
 */
void window_size_callback(GLFWwindow * window, int width, int height){
	mctx.window_ctx.n_width = width;
	mctx.window_ctx.n_height = height;

	float t_w = width;
	float t_h = (width/(mctx.width/mctx.height));
	if (t_h > height){
		t_h = height;
		t_w = (t_h * (mctx.width/mctx.height));
	}

	mctx.window_ctx.padx = ((width - t_w)/2.0f);
	mctx.window_ctx.pady = ((height - t_h)/2.0f);
	glViewport(mctx.window_ctx.padx, mctx.window_ctx.pady, t_w, t_h);
	glm_ortho(0, mctx.width, mctx.height, 0, -1.0f, 1.0f, Rend.projection);

}
