using System.Runtime.CompilerServices;
using System.Collections;
using System;

namespace VisualKey {

  public class Stash {
#pragma warning disable 0169
    private Vec3 position;
    private Quat rotation;
#pragma warning restore 0169

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void NewStash(Stash recipient);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void PopStash(Stash stash);

    public Stash() {
      NewStash(this);
    }

    public void Pop() {
      PopStash(this);
    }
  }

  public class Mesh {
#pragma warning disable 0169
    // private uint vao;
    private uint vboArray;
    private uint vboElement;
    private uint verticesCount;
    private uint indicesCount;
    private bool isLine;
#pragma warning restore 0169

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DrawMesh(Mesh mesh);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void CreateMesh(float[] vertices, bool isLine, Mesh recipient);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void CreateMeshIndices(float[] vertices, uint[] indices, bool isLine, Mesh recipient);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DestroyMesh(Mesh mesh);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void Translate(Vec3 position);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void Rotate(Vec3 rotation);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void RotateX(float rotation);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void RotateY(float rotation);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void RotateZ(float rotation);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void PolygonMode(bool mode);

    public Mesh(float[] vertices, bool isLine = false) {
      CreateMesh(vertices, isLine, this);
    }

    public Mesh(float[] vertices, uint[] indices, bool isLine = false) {
      CreateMeshIndices(vertices, indices, isLine, this);
    }

    ~Mesh() {
      DestroyMesh(this);
    }

    public void Draw() {
      DrawMesh(this);
    }
  }

  public class Line {
    public Mesh mesh { get; private set; }

    private void CreateLine(float p0x, float p0y, float p1x, float p1y) {
      float[] vertices = {
        p0x, p0y, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        p1x, p1y, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f
      };
      mesh = new Mesh(vertices, true);
    }

    public Line(float p0x, float p0y, float p1x, float p1y) {
      CreateLine(p0x, p0y, p1x, p1y);
    }

    public Line(Vec2 p0, Vec2 p1) {
      CreateLine(p0.x, p0.y, p1.x, p1.y);
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class Rectangle {
    public Mesh mesh { get; private set; }

    public Rectangle(float width, float height) {
      float[] vertices = {
         (width / 2.0f),  (height / 2.0f), 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
         (width / 2.0f), -(height / 2.0f), 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -(width / 2.0f), -(height / 2.0f), 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -(width / 2.0f),  (height / 2.0f), 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f
      };
      uint[] indices = {
        0, 1, 3,
        1, 2, 3
      };
      mesh = new Mesh(vertices, indices);
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class Triangle {
    public Mesh mesh { get; private set; }

    public Triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
      float[] vertices = {
        x1, y1, 0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f,
        x2, y2, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        x3, y3, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f
      };
      mesh = new Mesh(vertices);
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class Quad {
    public Mesh mesh { get; private set; }

    public Quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
      float[] vertices = {
        x1, y1, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
        x2, y2, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        x3, y3, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        x4, y4, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f
      };
      uint[] indices = {
        0, 1, 3,
        1, 2, 3
      };
      mesh = new Mesh(vertices, indices);
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class Circle {
    public Mesh mesh { get; private set; }

    public Circle(float radius, uint steps) {
      float[] vertices = new float[(steps + 1) * 8];
      uint[] indices = new uint[steps * 3];

      for (int n = 0; n < (steps + 1) * 8; ++n)
        vertices[n] = 0.0f;

      vertices[3] = 0.5f;
      vertices[4] = 0.5f;

      var increment = 2.0f * MathF.PI / (steps - 1);
      var i = 1;
      for (float angle = 0.0f; angle <= 2.0f * MathF.PI; angle += increment) {
        var x = (radius * MathF.Cos(angle)) / 2.0f;
        var y = (radius * MathF.Sin(angle)) / 2.0f;
        vertices[(i * 8)] = x;
        vertices[(i * 8) + 1] = y;
        var tx = (x + (radius / 2.0f)) / radius;
        var ty = (y + (radius / 2.0f)) / radius;
        vertices[(i * 8) + 3] = tx;
        vertices[(i * 8) + 4] = ty;

        vertices[(i * 8) + 5] = 0.0f;
        vertices[(i * 8) + 6] = 0.0f;
        vertices[(i * 8) + 7] = 0.0f;
        i++;
      }

      for (uint n = 0; n < steps - 1; ++n) {
        indices[(n * 3)] = 0;
        indices[(n * 3) + 1] = n + 1;
        indices[(n * 3) + 2] = n + 2;
      }
      indices[((steps - 1) * 3) + 1] = steps;
      indices[((steps - 1) * 3) + 2] = 1;

      mesh = new Mesh(vertices, indices);
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class Cube {
    public Mesh mesh { get; private set; }

    public Cube(float width, float height, float depth) {
      float[] vertices = {
        -(width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         (width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         (width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         (width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -(width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -(width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -(width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
         (width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
         (width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
         (width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        -(width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        -(width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

        -(width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
        -(width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -(width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -(width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -(width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
        -(width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

         (width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         (width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         (width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         (width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         (width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         (width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -(width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         (width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         (width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         (width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -(width / 2.0f), -(height / 2.0f),  (depth / 2.0f),  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -(width / 2.0f), -(height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -(width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         (width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         (width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         (width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -(width / 2.0f),  (height / 2.0f),  (depth / 2.0f),  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -(width / 2.0f),  (height / 2.0f), -(depth / 2.0f),  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
      };

      mesh = new Mesh(vertices);
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class Sphere {
    public Mesh mesh { get; private set; }

    public Sphere(float radius, uint sectors, uint stacks) {
      ArrayList vertices = new ArrayList();
      // ArrayList<float> normals = new ArrayList();
      // ArrayList<float> texture = new ArrayList();
      ArrayList indices = new ArrayList();

      float x, y, z, xy;
      float nx, ny, nz, lengthInv = 1.0f / radius;
      float u, v;

      float sectorStep = 2.0f * MathF.PI / sectors;
      float stackStep = MathF.PI / stacks;
      float sectorAngle, stackAngle;

      for (int i = 0; i <= stacks; ++i) {
        stackAngle = MathF.PI / 2 - i * stackStep;
        xy = radius * MathF.Cos(stackAngle);
        z = radius * MathF.Sin(stackAngle);

        for (int j = 0; j<= sectors; ++j) {
          sectorAngle = j * sectorStep;

          x = xy * MathF.Cos(sectorAngle);
          y = xy * MathF.Sin(sectorAngle);
          vertices.Add(x);
          vertices.Add(y);
          vertices.Add(z);

          u = (float)j / sectors;
          v = (float)i / stacks;
          vertices.Add(u);
          vertices.Add(v);

          nx = x * lengthInv;
          ny = y * lengthInv;
          nz = z * lengthInv;
          vertices.Add(nx);
          vertices.Add(ny);
          vertices.Add(nz);
        }
      }

      uint k1, k2;
      for (int i = 0; i < stacks; ++i) {
        k1 = (uint)i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
          if (1 != 0) {
            indices.Add(k1);
            indices.Add(k2);
            indices.Add(k1 + 1);
          }
          if (i != stacks - 1) {
            indices.Add(k1 + 1);
            indices.Add(k2);
            indices.Add(k2 + 1);
          }
        }
      }

      mesh = new Mesh((float[])vertices.ToArray(typeof(float)), (uint[])indices.ToArray(typeof(uint)));
    }

    public void Draw() {
      mesh.Draw();
    }
  }

  public class QuadraticBezier {

    Vec2 p0, p1, p2;

    public QuadraticBezier(Vec2 p0, Vec2 p1, Vec2 p2) {
      this.p0 = p0;
      this.p1 = p1;
      this.p2 = p2;
    }

    public void Step(float t, ref Vec2 res) {
      res.x = MathF.Pow(1 - t, 2) * p0.x + (1 - t) * 2 * t * p1.x + t * t * p2.x;
      res.y = MathF.Pow(1 - t, 2) * p0.y + (1 - t) * 2 * t * p1.y + t * t * p2.y;
    }

    public void Draw(uint steps) {
      Vec2[] bezierSteps = new Vec2[steps];
      for (int i = 0; i < bezierSteps.Length; ++i) {
        float t = (float)i / (float)(bezierSteps.Length - 1);
        bezierSteps[i] = new Vec2(0);
        this.Step(t, ref bezierSteps[i]);
      }
      for (int i = 0; i < bezierSteps.Length - 1; ++i) {
        Line l = new Line(bezierSteps[i], bezierSteps[i + 1]);
        l.Draw();
      }
    }

  }

  public class CubicBezier {

    Vec2 p0, p1, p2, p3;

    public CubicBezier(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3) {
      this.p0 = p0;
      this.p1 = p1;
      this.p2 = p2;
      this.p3 = p3;
    }

    public void Step(float t, ref Vec2 res) {
      res.x = MathF.Pow(1 - t, 3) * p0.x + MathF.Pow(1 - t, 2) * 3 * t * p1.x + (1 - t) * 3 * t * t * p2.x + t * t * t * p3.x;
      res.y = MathF.Pow(1 - t, 3) * p0.y + MathF.Pow(1 - t, 2) * 3 * t * p1.y + (1 - t) * 3 * t * t * p2.y + t * t * t * p3.y;
    }

    public void Draw(uint steps) {
      Vec2[] bezierSteps = new Vec2[steps];
      for (int i = 0; i < bezierSteps.Length; ++i) {
        float t = (float)i / (float)(bezierSteps.Length - 1);
        bezierSteps[i] = new Vec2(0);
        this.Step(t, ref bezierSteps[i]);
      }
      for (int i = 0; i < bezierSteps.Length - 1; ++i) {
        Line l = new Line(bezierSteps[i], bezierSteps[i + 1]);
        l.Draw();
      }
    }

  }

  public class Light {

    private uint id;

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void EnableLightSystem();
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DisableLightSystem();
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static uint CreateLightSource(Vec3 position);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void DestroyLightSource(uint id);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void ChangePosition(uint id, Vec3 position);

    private Vec3 _position;
    public Vec3 position {
      get {
        return _position;
      }
      set {
        ChangePosition(this.id, value);
        this._position = value;
      }
    }

    public static void Enable() {
      EnableLightSystem();
    }

    public static void Disable() {
      DisableLightSystem();
    }

    public Light(Vec3 pos) {
      this.id = CreateLightSource(pos);
    }

    ~Light() {
      DestroyLightSource(this.id);
    }

  }

}
