#pragma once

#include "typedefs.h"

struct GLFWwindow;

namespace visualkey {

  struct WindowData {
    GLFWwindow *window = nullptr;
  };

  WindowData* WindowCreate(u32 width, u32 height, std::string title); // Reverse name due to Windows already cotaining a func with that name
  GLFWwindow* GetFocusedWindow();
  GLFWwindow* GetDefaultWindow();
  void DestroyWindow(WindowData *data);
  void MakeCurrent(WindowData *data);
  void InitGFX();
  void NewFrame();
  void SwapBuffers(GLFWwindow *window);
  void SwapAllBuffers();
  void TerminateGFX();
  void ClearBackground(u32 r, u32 g, u32 b);
  void CloseWindow();
  void WindowSize(u32 width, u32 height);
  void SetOrtho(bool mode);
  bool WindowFullscreen();
  bool WindowIsOpen(GLFWwindow *window);
  bool IsOrtho();
  f32 DeltaTime();
  v2i GetWindowSize(WindowData *data);

}
