#pragma once

#include "types.h"

struct GLFWwindow;

namespace visualkey {

  struct WindowData {
    u32 id             = 0;
    GLFWwindow *window = nullptr;
  };

  WindowData *WindowCreate(
    u32 width,
    u32 height,
    std::string title); // Reverse name due to Windows already cotaining a func with that name
  WindowData *GetWindowsFirst();
  GLFWwindow *GetFocusedWindow();
  GLFWwindow *GetDefaultWindow();
  GLFWwindow *GetWindowFromID(u32 id);
  void DestroyWindow(WindowData *data);
  void MakeCurrent(WindowData *data);
  void InitGFX();
  void NewFrame();
  void SwapBuffers(GLFWwindow *window);
  void SwapAllBuffers();
  void TerminateGFX();
  void ClearBackground(u32 r, u32 g, u32 b);
  void CloseCurrentWindow();
  void CloseWindow(WindowData *data);
  void WindowSize(WindowData *data, u32 width, u32 height);
  void SetOrtho();
  void SetPerspective(f32 fov);
  bool WindowFullscreen();
  bool WindowIsOpen(GLFWwindow *window);
  bool IsOrtho();
  f32 DeltaTime();
  v2i GetWindowSize(WindowData *data);

}
