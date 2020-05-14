#include "iomanager.h"

namespace visualkey {

bool keys[(i32)Key::Last];
bool mouse[(i32)MouseButton::Last];
bool pad[(i32)Pad::Last][(i32)PadButton::Last];
bool rKeys[(i32)Key::Last];
bool rMouse[(i32)MouseButton::Last];
bool rPad[(i32)Pad::Last][(i32)PadButton::Last];
bool pKeys[(i32)Key::Last];
bool pMouse[(i32)MouseButton::Last];
bool pPad[(i32)Pad::Last][(i32)PadButton::Last];
v2d mousePos;
float joySticks[(i32)JoyStick::Last];

void
UpdatePads() {
  for (i32 j = 0; j < (i32)Pad::Last; ++j) {
    if (!PadPresent((Pad)j))
      continue;

    GLFWgamepadstate state;
    if (!glfwGetGamepadState(j, &state))
      return;

    int count;
    const float* axes = glfwGetJoystickAxes(j, &count);
    if (count > (i32)JoyStick::Last)
      count = (i32)JoyStick::Last;
    for (int n = 0; n < count; ++n)
      joySticks[n] = axes[n];

    for (i32 i = 0; i < (i32)PadButton::Last; ++i) {
      bool pressed = state.buttons[i];
      if (pad[j][i] && !pressed)
        rPad[j][i] = true;
      else if (!pad[j][i] && pressed)
        pPad[j][i] = true;
      pad[j][i] = pressed;
    }
  }
}

bool
PadPresent(Pad pad) {
  return glfwJoystickPresent((i32)pad);
}

void
KeyEvent(i32 key, bool pressed) {
  if (keys[key] && !pressed)
    rKeys[key] = true;
  else if (!keys[key] && pressed)
    pKeys[key] = true;
  keys[key] = pressed;
}

void
MouseButtonEvent(i32 button, bool pressed) {
  if (mouse[button] && !pressed)
    rMouse[button] = true;
  else if (!mouse[button] && pressed)
    pMouse[button] = true;
  mouse[button] = pressed;
}

void
MouseEvent(f64 x, f64 y) {
  mousePos.x = x;
  mousePos.y = y;
}

bool
IsKeyPressed(i32 key) {
  return keys[key];
}

bool
IsMousePressed(i32 button) {
  return mouse[button];
}

bool
IsPadPressed(i32 padNum, i32 button) {
  return pad[padNum][button];
}

bool
IsKeyJustPressed(i32 key) {
  if (!pKeys[key])
    return false;
  pKeys[key] = false;
  return true;
}

bool
IsMouseJustPressed(i32 button) {
  if (!pMouse[button])
    return false;
  pMouse[button] = false;
  return true;
}

bool
IsPadJustPressed(i32 pad, i32 button) {
  if (!pPad[pad][button])
    return false;
  pPad[pad][button] = false;
  return true;
}

bool
IsKeyJustReleased(i32 key) {
  if (!rKeys[key])
    return false;
  rKeys[key] = false;
  return true;
}

bool
IsMouseJustReleased(i32 button) {
  if (!rMouse[button])
    return false;
  rMouse[button] = false;
  return true;
}

bool
IsPadJustReleased(i32 pad, i32 button) {
  if (!rPad[pad][button])
    return false;
  rPad[pad][button] = false;
  return true;
}

v2d
GetMousePos() {
  return mousePos;
}

float
GetJoyStick(JoyStick joyStick) {
  return joySticks[(i32)joyStick];
}

}