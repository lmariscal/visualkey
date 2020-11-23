using System.Runtime.CompilerServices;
using System;

namespace VisualKey {

  public class Window {
#pragma warning disable 0169
    private IntPtr window;
#pragma warning restore 0169

    public Vec2 size {
      get {
        return GetSizeWindow(this);
      }
      set {
        ResizeWindow((uint)value.x, (uint)value.y, this);
      }
    }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void BackgroundWindow(uint r, uint g, uint b);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void ResizeWindow(uint width, uint height, Window window);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static Vec2 GetSizeWindow(Window window);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool FullscreenWindow();
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void MakeCurrentWindow(Window window);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void CreateWindow(uint width, uint height, string title, Window recipient);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DestroyWindow(Window window);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetOrtho(bool mode);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool GetOrtho();

    public static void SetOrtho() {
      SetOrtho(true);
    }

    public static void SetPerspective() {
      SetOrtho(false);
    }

    public static bool IsOrtho() {
      return GetOrtho();
    }

    public static bool IsPerspective() {
      return !GetOrtho();
    }

    private static void BackgroundWindow(Vec3 color) {
      BackgroundWindow((uint)color.x, (uint)color.y, (uint)color.z);
    }

    public Window(uint width, uint height, string title = "VisualKey") {
      CreateWindow(width, height, title, this);
    }

    ~Window() {
      DestroyWindow(this);
    }

    public void Background(uint r, uint g, uint b) {
      this.MakeCurrent();
      BackgroundWindow(r, g, b);
    }

    public void Background(Vec3 color) {
      this.MakeCurrent();
      BackgroundWindow(color);
    }

    public bool Fullscreen() {
      this.MakeCurrent();
      return FullscreenWindow();
    }

    public void MakeCurrent() {
      MakeCurrentWindow(this);
    }
  }

}
