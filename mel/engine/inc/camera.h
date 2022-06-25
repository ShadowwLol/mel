#ifndef _MEL_CAMERA_H
#define _MEL_CAMERA_H

#include "mel.h"

typedef struct {
  mat4 view;
  vec3 pos;
  vec3 scale;
  GLfloat rotation;
} MEL_Camera;

#define MEL_init_camera(camera){\
	camera.pos[0]   = 0.0f;\
	camera.pos[1]   = 0.0f;\
	camera.pos[2]   = 0.0f;\
	camera.scale[0] = 1.0f;\
	camera.scale[1] = 1.0f;\
	camera.scale[2] = 0.0f;\
	camera.rotation = 0.0f;\
	glm_mat4_identity(camera.view);\
}

#define MEL_reset_camera(camera) glm_mat4_identity(camera.view)

#define MEL_update_camera(camera){\
	MEL_reset_camera(camera);\
	glm_translate(camera.view, (vec3){camera.pos[0]*-1, camera.pos[1]*-1, camera.pos[2]*-1});\
	glm_scale(camera.view, camera.scale);\
	glm_rotate(camera.view, glm_rad(camera.rotation), (vec3){0.0f, 0.0f, 1.0f});\
}
#endif
