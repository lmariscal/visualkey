using System.Runtime.CompilerServices;
using System;

namespace VisualKey {

  public class Texture {
#pragma warning disable 0169
    private uint id;
#pragma warning restore 0169

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void CreateTexture(string path, Texture recipient);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DestroyTexture(Texture texture);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DrawTexture(Texture texture);

    public Texture(string path) {
      CreateTexture(path, this);
    }

    ~Texture() {
      DestroyTexture(this);
    }

    public void Draw() {
      DrawTexture(this);
    }
  }

  public class Color {
    public Vec3 color { get; private set; }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DrawColor(Vec3 color);

    public Color(Vec3 color) {
      this.color = color / 255.0f;
    }

    public void SetColor(Vec3 color) {
      this.color = color / 255.0f;
    }

    public void Draw() {
      DrawColor(color);
    }
  }

}
