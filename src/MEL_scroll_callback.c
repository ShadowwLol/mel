#include "../include/MEL_opengl.h"
#include "../include/MEL_Texture.h"

extern MEL_Texture smiley;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	switch((int)yoffset){
		case -1:
			smiley.rect.size[0] -= 10;
			smiley.rect.size[1] -= 10;
			break;
		case 1:
			smiley.rect.size[0] += 10;
			smiley.rect.size[1] += 10;
			break;
		default:
			break;
	}
}
