using System.Runtime.CompilerServices;
using System;

namespace VisualKey {

  public class Shader {
#pragma warning disable 0169
    private uint program;
#pragma warning restore 0169

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void CreateShader(string path, Shader recipient);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DestroyShader(Shader shader);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DrawShader(Shader shader);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static int GetLocation(Shader shader, string name);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetInt(Shader shader, int location, int val);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetVec2(Shader shader, int location, Vec2 val);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetVec3(Shader shader, int location, Vec3 val);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetVec4(Shader shader, int location, Vec4 val);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetMat3(Shader shader, int location, Mat3 val);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetMat4(Shader shader, int location, Mat4 val);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static Shader GetUberShader();

    public Shader(string path) {
      CreateShader(path, this);
    }

    ~Shader() {
      DestroyShader(this);
    }

    public void Draw() {
      DrawShader(this);
    }

    public int GetLocation(string name) {
      return GetLocation(this, name);
    }

    public uint GetProgram() {
      return program;
    }

    public void SetInt(int location, int val) {
      SetInt(this, location, val);
    }

    public void SetVec2(int location, Vec2 val) {
      SetVec2(this, location, val);
    }

    public void SetVec3(int location, Vec3 val) {
      SetVec3(this, location, val);
    }

    public void SetVec4(int location, Vec4 val) {
      SetVec4(this, location, val);
    }

    public void SetMat3(int location, Mat3 val) {
      SetMat3(this, location, val);
    }

    public void SetMat4(int location, Mat4 val) {
      SetMat4(this, location, val);
    }
  }

}
