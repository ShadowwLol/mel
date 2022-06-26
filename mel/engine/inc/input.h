#ifndef _INPUT_H
#define _INPUT_H

#include "common.h"

#define KEYBOARD_SIZE (512)

#define MEL_NONE    (0)
#define MEL_PRESS   (1)
#define MEL_REPEAT  (2)
#define MEL_RELEASE (3)

typedef struct {
  i32 index;                    // Joystick index
  const char* name;             // Joystick name
  const unsigned char* buttons; // Joystick buttons
  const unsigned char* hats;    // Joystick hats
  const f32* axes;              // Joystick analog axes
  i32* buttons_count;           // Number of joystick buttons joystick has
  i32* axes_count;              // Number of axes joystick has
  i32* hats_count;              // Number of hats joystick has
} joystick_t;

static i16 keyboard[KEYBOARD_SIZE];

static void mel_init_joysticks(joystick_t * joysticks[], size_t amount);
static void mel_joysticks(joystick_t * joysticks[], size_t amount);

static void init_keyboard(void);
static void reset_keyboard(void);
static i8 mel_key(i16 key);

inline static i8 mel_key(i16 key) {
  return keyboard[key];
}

inline static void mel_init_joysticks(joystick_t * joysticks[], size_t amount) {
  for (size_t i = 0; i < amount; ++i) {
    joysticks[i]->index = i;
  }
}

inline static void mel_joysticks(joystick_t * joysticks[], size_t amount) {
  for (size_t i = 0; i < amount; ++i) {
    joysticks[i]->name = glfwGetJoystickName(joysticks[i]->index);
    joysticks[i]->buttons =
      glfwGetJoystickButtons(joysticks[i]->index,
                             joysticks[i]->buttons_count);
    joysticks[i]->axes =
      glfwGetJoystickAxes(joysticks[i]->index, joysticks[i]->axes_count);
    joysticks[i]->hats =
      glfwGetJoystickHats(joysticks[i]->index, joysticks[i]->hats_count);
  }

}

inline static void init_keyboard(void) {
  for (u16 i = 0; i < KEYBOARD_SIZE; ++i) {
    keyboard[i] = MEL_NONE;
  }
}

inline static void reset_keyboard(void) {
  for (u16 i = 0; i < KEYBOARD_SIZE; ++i) {
    if (keyboard[i] == MEL_RELEASE) {
      keyboard[i] = MEL_NONE;
    }
  }
}

inline static void _input(GLFWwindow * window, i32 key, i32 scancode,
                          i32 action, i32 mods) {
  switch (action) {
    default:
      keyboard[key] = MEL_NONE;
      break;
    case GLFW_RELEASE:
      keyboard[key] = MEL_RELEASE;
      break;
    case GLFW_PRESS:
      keyboard[key] = MEL_PRESS;
      break;
    case GLFW_REPEAT:
      keyboard[key] = MEL_REPEAT;
      break;
  }
}

#endif /* _INPUT_H */
