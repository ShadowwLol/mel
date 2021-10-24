#include "../include/MEL_opengl.h"
#include "../include/MEL_logs.h"

#if __WIN32
#include <windows.h>
extern HANDLE hConsole;
extern WORD saved_attributes;
#endif

void joystick_callback(int jid, int event){
	switch (event){
		case GLFW_CONNECTED:
			log_log(LOG_INFORMATION, "Controller [%d]{%s} connected!", jid, glfwGetGamepadName(jid));
			break;
		case GLFW_DISCONNECTED:
			log_log(LOG_INFORMATION, "Controller [%d]{%s} disconnected!", jid, glfwGetGamepadName(jid));
			break;
	}
}
