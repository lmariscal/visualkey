#include "window.h"

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <vector>

#include "iomanager.h"
#include "icon.h"
#include "shader.h"

#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#define MIN(a, b) (((b) < (a)) ? (b) : (a))

namespace visualkey {

  f32 last_time              = 0;
  f32 delta_time             = 0;
  f32 perspective_fov        = radians(90.0f);
  GLFWwindow *default_window = nullptr;
  GLFWwindow *focused_window = nullptr;
  std::vector<WindowData *> windows;
  std::vector<u32> destroyed_windows;
  bool is_ortho = true;

  WindowData *
  GetWindowsFirst() {
    for (WindowData *wd : windows) {
      if (wd->window && wd->id != 0) return wd;
    }
    return nullptr;
  }

  void
  SetOrtho() {
    is_ortho = true;
  }

  void
  SetPerspective(f32 fov) {
    is_ortho        = false;
    perspective_fov = fov;
  }

  bool
  IsOrtho() {
    return is_ortho;
  }

  GLFWmonitor *
  GetBestMonitor(GLFWwindow *window) {
    i32 monitor_count;
    GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);

    if (!monitors) return nullptr;

    i32 window_x, window_y, window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    glfwGetWindowPos(window, &window_x, &window_y);

    GLFWmonitor *best_monitor = nullptr;
    i32 best_area             = 0;

    for (i32 i = 0; i < monitor_count; ++i) {
      GLFWmonitor *monitor = monitors[i];

      i32 monitor_x, monitor_y;
      glfwGetMonitorPos(monitor, &monitor_x, &monitor_y);

      const GLFWvidmode *mode = glfwGetVideoMode(monitor);
      if (!mode) continue;

      i32 area_min_x = MAX(window_x, monitor_x);
      i32 area_min_y = MAX(window_y, monitor_y);

      i32 area_max_x = MIN(window_x + window_width, monitor_x + mode->width);
      i32 area_max_y = MIN(window_y + window_height, monitor_y + mode->height);

      i32 area = (area_max_x - area_min_x) * (area_max_y - area_min_y);

      if (area > best_area) {
        best_area    = area;
        best_monitor = monitor;
      }
    }

    return best_monitor;
  }

  void
  WindowSizeEvent(GLFWwindow *window, i32 width, i32 height) {
    glViewport(0, 0, width, height);
  }

  bool
  WindowFullscreen() {
    GLFWwindow *window      = glfwGetCurrentContext();
    GLFWmonitor *current    = glfwGetWindowMonitor(window);
    GLFWmonitor *monitor    = GetBestMonitor(window);
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    if (current) {
      glfwSetWindowMonitor(window,
                           nullptr,
                           (mode->width / 2) - (1280 / 2),
                           (mode->height / 2) - (720 / 2),
                           1280,
                           720,
                           GLFW_DONT_CARE);
      return false;
    }

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
    return true;
  }

  void
  GLFWErrorCallback(i32 error, const char *description) {
    std::cerr << description << "\n";
  }

  void
  KeyEvent(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods) {
    if (action != GLFW_REPEAT) KeyEvent(key, action != GLFW_RELEASE);
  }

  void
  CharEvent(GLFWwindow *window, unsigned int c) {}

  void
  MouseButtonEvent(GLFWwindow *window, i32 button, i32 action, i32 mods) {
    MouseButtonEvent(button, action != GLFW_RELEASE);
  }

  void
  ScrollEvent(GLFWwindow *window, double xoffset, double yoffset) {}

  void
  MouseEvent(GLFWwindow *window, f64 x, f64 y) {
    if (!glfwGetWindowAttrib(window, GLFW_HOVERED)) return;
    MouseEvent(x, y);
  }

  void
  FocusEvent(GLFWwindow *window, i32 focused) {
    if (focused) focused_window = window;
  }

  WindowData *
  WindowCreate(u32 width, u32 height, std::string title) {
    WindowData *data = new WindowData();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    data->window = glfwCreateWindow(width, height, title.c_str(), nullptr, default_window);
    if (!data->window) {
      std::cerr << "Failed to create Window\n";
      delete data;
      return nullptr;
    }

    glfwMakeContextCurrent(data->window);
    data->id = windows.size() + 1;

    GLFWimage image;
    image.pixels = stbi_load_from_memory(icon, 4999, &image.width, &image.height, nullptr, 4);
    glfwSetWindowIcon(data->window, 1, &image);
    stbi_image_free(image.pixels);

    GLFWmonitor *monitor    = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(data->window,
                     (mode->width / 2) - (width / 2),
                     (mode->height / 2) - (height / 2));

    glfwSetKeyCallback(data->window, KeyEvent);
    glfwSetCharCallback(data->window, CharEvent);
    glfwSetMouseButtonCallback(data->window, MouseButtonEvent);
    glfwSetScrollCallback(data->window, ScrollEvent);
    glfwSetCursorPosCallback(data->window, MouseEvent);
    glfwSetWindowSizeCallback(data->window, WindowSizeEvent);
    glfwSetWindowFocusCallback(data->window, FocusEvent);

    vec<2, f64> mouse_pos;
    glfwGetCursorPos(data->window, &mouse_pos.x, &mouse_pos.y);
    MouseEvent(mouse_pos.x, mouse_pos.y);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FocusEvent(data->window, 1);

#if defined(VISUALKEY_DEBUG)
    std::cout << "Created Window: " << data->id << '\n';
#endif
    WindowData *copy = new WindowData();
    copy->window     = data->window;
    copy->id         = data->id;
    windows.push_back(copy);

    return data;
  }

  void
  MakeCurrent(WindowData *data) {
    if (!data->window) return;
    if (!WindowIsOpen(data->window)) return;
    glfwMakeContextCurrent(data->window);

    v2i window_size(0);
    glfwGetWindowSize(data->window, &window_size.x, &window_size.y);
    glViewport(0, 0, window_size.x, window_size.y);

    ShaderData *current_shader = GetCurrentShader();
    if (current_shader->program == GetUberShader()) {
      v2i size(0);
      glfwGetWindowSize(glfwGetCurrentContext(), &size.x, &size.y);
      m4 perspective = IsOrtho()
        ? ortho(-(size.x / 2.0f), size.x / 2.0f, -(size.y / 2.0f), size.y / 2.0f)
        : glm::perspective(perspective_fov, (f32)size.x / (f32)size.y, 0.1f, 100.0f);

      i32 perspective_loc = GetLocation(current_shader, "Projection");
      SetMat4(current_shader, perspective_loc, perspective);
    }
  }

  GLFWwindow *
  GetWindowFromID(u32 id) {
    GLFWwindow *result = nullptr;
    for (WindowData *data : windows) {
      if (!data->window) continue;
      if (id != data->id) continue;
      result = data->window;
      break;
    }
    return result;
  }

  void
  DestroyWindow(WindowData *data) {
    if (!data->window) return;
    if (glfwGetCurrentContext() == data->window) glfwMakeContextCurrent(default_window);

#if defined(VISUALKEY_DEBUG)
    std::cout << "Destroying window: " << data->id << '\n';
#endif

    // bool is_destroyed = false;
    // for (u32 other_id : destroyed_windows)
    //   if (other_id == data->id) is_destroyed = true;
    // if (is_destroyed) return;

    glfwDestroyWindow(data->window);

    bool one_open = false;
    for (WindowData *window_data : windows) {
      if (!window_data->window) continue;
      if (WindowIsOpen(window_data->window)) one_open = true;
      if (data->id != window_data->id) continue;
      window_data->window = nullptr;
    }

    if (!one_open) glfwSetWindowShouldClose(GetDefaultWindow(), true);
  }

  void
  InitGFX() {
    if (glfwInit() != GLFW_TRUE) {
      std::cerr << "Failed to init GLFW\n";
      return;
    }
    glfwSetErrorCallback(GLFWErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    default_window = glfwCreateWindow(1280, 720, "VisualKey", nullptr, nullptr);
    if (!default_window) {
      std::cerr << "Failed to create hidden Window\n";
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(default_window);

    GLFWimage image;
    image.pixels = stbi_load_from_memory(icon, 4999, &image.width, &image.height, nullptr, 4);
    glfwSetWindowIcon(default_window, 1, &image);
    stbi_image_free(image.pixels);

    GLFWmonitor *monitor    = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(default_window,
                     (mode->width / 2) - (1280 / 2),
                     (mode->height / 2) - (720 / 2));

    glfwSetKeyCallback(default_window, KeyEvent);
    glfwSetCharCallback(default_window, CharEvent);
    glfwSetMouseButtonCallback(default_window, MouseButtonEvent);
    glfwSetScrollCallback(default_window, ScrollEvent);
    glfwSetCursorPosCallback(default_window, MouseEvent);
    glfwSetWindowSizeCallback(default_window, WindowSizeEvent);
    glfwSetWindowFocusCallback(default_window, FocusEvent);

    if (gladLoadGL() != GL_TRUE) {
      std::cerr << "Failed to load OpenGL\n";
      glfwDestroyWindow(default_window);
      glfwTerminate();
      return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  GLFWwindow *
  GetFocusedWindow() {
    return focused_window;
  }

  GLFWwindow *
  GetDefaultWindow() {
    return default_window;
  }

  void
  NewFrame() {
    bool one_window_open        = false;
    GLFWwindow *current_context = glfwGetCurrentContext();

    for (WindowData *data : windows) {
      if (!data->window) continue;
      if (!WindowIsOpen(data->window)) continue;
      one_window_open = true;

      glfwMakeContextCurrent(data->window);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwMakeContextCurrent(current_context);
    if (!one_window_open) glfwSetWindowShouldClose(default_window, true);

    glfwPollEvents();
    UpdatePads();

    for (WindowData *data : windows) {
      if (!data->window) continue;
      if (WindowIsOpen(data->window)) continue;
      DestroyWindow(data);
    }

    f32 now    = glfwGetTime();
    delta_time = now - last_time;
    last_time  = now;
  }

  void
  SwapBuffers(GLFWwindow *window) {
    if (!WindowIsOpen(window)) return;
    glfwSwapBuffers(window);
  }

  void
  SwapAllBuffers() {
    for (WindowData *data : windows) {
      if (!data->window) continue;
      SwapBuffers(data->window);
    }
  }

  void
  TerminateGFX() {
    glfwDestroyWindow(default_window);
    glfwTerminate();

    for (WindowData *data : windows) {
      if (data->window) DestroyWindow(data);
      delete data;
    }
  }

  void
  ClearBackground(u32 r, u32 g, u32 b) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
  }

  void
  CloseWindow(WindowData *data) {
    if (!data->window) return;
    glfwSetWindowShouldClose(data->window, true);
  }

  void
  CloseCurrentWindow() {
    GLFWwindow *window = glfwGetCurrentContext();
    glfwSetWindowShouldClose(window, true);
  }

  bool
  WindowIsOpen(GLFWwindow *window) {
    if (!window) return false;
    return !glfwWindowShouldClose(window);
  }

  f32
  DeltaTime() {
    return delta_time;
  }

  void
  WindowSize(WindowData *data, u32 width, u32 height) {
    if (!data->window) return;
    glfwSetWindowSize(data->window, width, height);
  }

  v2i
  GetWindowSize(WindowData *data) {
    v2i result(0);
    if (!data->window) return result;
    glfwGetWindowSize(data->window, &result.x, &result.y);
    return result;
  }

}
