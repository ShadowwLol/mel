#include "../include/MEL_def.h"
#include "../include/MEL_opengl.h"
#include "../include/MEL_misc.h"
#include "../include/MEL_Camera.h"

#include "../include/MEL_Texture.h"
extern MEL_Texture smiley;
extern MEL_Camera camera;
extern MEL_Window win;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	/* Controller Handling */
	GLFWgamepadstate state;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state)){
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]){
			camera.pos[1] -= 10;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]){
			camera.pos[0] -= 10;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]){
			camera.pos[1] += 10;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]){
			camera.pos[0] += 10;
		}
	}
	/* * * * * * * * * * * */
	switch(action){
		case GLFW_RELEASE:
			switch(key){
				case GLFW_KEY_ESCAPE:
					/* Close the game */
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
				case GLFW_KEY_F11:
					/* Fullscreen the game */
					toggleFullscreen(win);
					break;
				case GLFW_KEY_SPACE:
					if (smiley.rect.color[0] == 1.0f){
						smiley.rect.color[0] = 0.0f;
					}else{
						smiley.rect.color[0] = 1.0;
					}
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
}
