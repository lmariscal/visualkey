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
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetNoneTexture();

    private static Color color = new Color(255, 255, 255);

    static public void SetNone() {
      SetNoneTexture();
      color.Draw();
    }

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
    public Vec4 color { get; private set; }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DrawColor(Vec4 color);

    public Color(Vec4 color) {
      this.color = Normalize(color);
    }

    public Color(Vec3 color) {
      this.color = new Vec4(color / 255.0f, 1.0f);
    }

    public Color(uint r, uint g, uint b, uint a = 100) {
      this.color = new Vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 100.0f);
    }

    public void SetColor(Vec4 color) {
      this.color = Normalize(color);
    }

    public void Draw() {
      DrawColor(color);
    }

    private Vec4 Normalize(Vec4 color) {
      Vec4 res = new Vec4(0);
      res.x = color.x / 255.0f;
      res.y = color.y / 255.0f;
      res.z = color.z / 255.0f;
      res.w = color.w > 1.0f ? color.w / 100.0f : color.w;
      return res;
    }
  }

}
