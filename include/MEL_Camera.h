#ifndef _MEL_CAMERA_H
#define _MEL_CAMERA_H

#include "MEL_opengl.h"

typedef struct {
	mat4 view;
	vec3 pos;
} MEL_Camera;

#define MEL_init_camera(camera){\
	camera.pos[0] = 0.0f;\
	camera.pos[1] = 0.0f;\
	camera.pos[2] = 0.0f;\
	glm_mat4_identity(camera.view);\
}

#define MEL_reset_camera(camera) glm_mat4_identity(camera.view)

#define MEL_update_camera(camera){\
	MEL_reset_camera(camera);\
	glm_translate(camera.view, (vec3){camera.pos[0]*-1, camera.pos[1]*-1, camera.pos[2]*-1});\
}
#endif
