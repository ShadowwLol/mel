#include "inc/mel.h"
#include "inc/def.h"
#include "inc/logs.h"

void joystick_callback(int jid, int event){
	switch (event){
		case GLFW_CONNECTED:
			MEL_log(LOG_INFORMATION, "Controller [%d]{%s} connected!", jid, glfwGetGamepadName(jid));
			break;
		case GLFW_DISCONNECTED:
			MEL_log(LOG_INFORMATION, "Controller [%d]{%s} disconnected!", jid, glfwGetGamepadName(jid));
			break;
	}
}