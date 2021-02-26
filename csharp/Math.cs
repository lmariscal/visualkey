using System;
using System.Runtime.InteropServices;

namespace VisualKey {

  public class Vec2 {
    public float x;
    public float y;

    public static Vec2 down() { return new Vec2(0, -1); }
    public static Vec2 left() { return new Vec2(-1, 0); }
    public static Vec2 up() { return new Vec2(0, 1); }
    public static Vec2 right() { return new Vec2(1, 0); }
    public static Vec2 zero() { return new Vec2(0); }
    public static Vec2 one() { return new Vec2(1); }

    public Vec2(float n) {
      this.x = n;
      this.y = n;
    }

    public Vec2(float x, float y) {
      this.x = x;
      this.y = y;
    }

    public Vec2(Vec3 v) {
      this.x = v.x;
      this.y = v.y;
    }

    public Vec2(Vec4 v) {
      this.x = v.x;
      this.y = v.y;
    }

    public float this[int i] {
      get {
        if (i == 0) return x;
        if (i == 1) return y;
        return 0;
      }
      set {
        if (i == 0) x = value;
        else if (i == 1) y = value;
      }
    }

    public override bool Equals(object other) {
      if (!(other is Vec2))
        return false;
      return ((Vec2)other).x == this.x && ((Vec2)other).y == this.y;
    }

    public override int GetHashCode() {
      return GetHashCode();
    }

    public static Vec2 operator +(Vec2 i, Vec2 j) {
      return new Vec2(i.x + j.x, i.y + j.y);
    }

    public static Vec2 operator -(Vec2 i, Vec2 j) {
      return new Vec2(i.x - j.x, i.y - j.y);
    }

    public static Vec2 operator *(Vec2 i, float k) {
      return new Vec2(i.x * k, i.y * k);
    }

    public static Vec2 operator /(Vec2 i, float k) {
      return new Vec2(i.x / k, i.y / k);
    }

    public static Vec2 operator +(Vec2 i, float k) {
      return new Vec2(i.x + k, i.y + k);
    }

    public static Vec2 operator -(Vec2 i, float k) {
      return new Vec2(i.x - k, i.y - k);
    }

    public static bool operator ==(Vec2 i, Vec2 j) {
      return Equals(i, j);
    }

    public static bool operator !=(Vec2 i, Vec2 j) {
      return !Equals(i, j);
    }

    public override string ToString() {
      return "(x: " + x + ", y: " + y + ")";
    }

    public void Set(float x, float y) {
      this.x = x;
      this.y = y;
    }

    public float MagnitudeSqrt() {
      return (x * x) + (y * y);
    }

    public float Magnitude() {
      return MathF.Sqrt(this.MagnitudeSqrt());
    }

    public Vec2 Normalize() {
      return this / this.Magnitude();
    }

    public float Dot(Vec2 other) {
      return (this.x * other.x) + (this.y * other.y);
    }

    public float Angle(Vec2 other) {
      var m = MathF.Abs(this.Dot(other));
      var d = this.Magnitude() * other.Magnitude();
      return MathF.Acos(m / d);
    }

    public float DistanceSqrt(Vec2 other) {
      return MathF.Pow(other.x - this.x, 2) + MathF.Pow(other.y - this.y, 2);
    }

    public float Distance(Vec2 other) {
      return MathF.Sqrt(this.DistanceSqrt(other));
    }
  }

  public class Vec3 {
    public float x;
    public float y;
    public float z;

    public static Vec3 back() { return new Vec3(0, 0, -1); }
    public static Vec3 down() { return new Vec3(0, -1, 0); }
    public static Vec3 left() { return new Vec3(-1, 0, 0); }
    public static Vec3 forward() { return new Vec3(0, 0, 1); }
    public static Vec3 up() { return new Vec3(0, 1, 0); }
    public static Vec3 right() { return new Vec3(1, 0, 0); }
    public static Vec3 zero() { return new Vec3(0); }
    public static Vec3 one() { return new Vec3(1); }

    public Vec3(float n) {
      this.x = n;
      this.y = n;
      this.z = n;
    }

    public Vec3(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
    }

    public Vec3(Vec2 v, float z) {
      this.x = v.x;
      this.y = v.y;
      this.z = z;
    }

    public Vec3(Vec4 v) {
      this.x = v.x;
      this.y = v.y;
      this.z = v.z;
    }

    public float this[int i] {
      get {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        return 0;
      }
      set {
        if (i == 0) x = value;
        else if (i == 1) y = value;
        else if (i == 2) z = value;
      }
    }

    public static Vec3 operator *(Vec3 v, Mat3 m) {
      Vec3 result = new Vec3(0);
      for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
          result[r] += m[r][c] * v[c];
      return result;
    }

    public override bool Equals(object obj) {
      if (!(obj is Vec3))
        return false;
      return ((Vec3)obj).x == this.x && ((Vec3)obj).y == this.y && ((Vec3)obj).z == this.z;
    }

    public override int GetHashCode() {
      return GetHashCode();
    }

    public static Vec3 operator +(Vec3 i, Vec3 j) {
      return new Vec3(i.x + j.x, i.y + j.y, i.z + j.z);
    }

    public static Vec3 operator -(Vec3 i, Vec3 j) {
      return new Vec3(i.x - j.x, i.y - j.y, i.z - j.z);
    }

    public static Vec3 operator *(Vec3 i, float k) {
      return new Vec3(i.x * k, i.y * k, i.z * k);
    }

    public static Vec3 operator /(Vec3 i, float k) {
      return new Vec3(i.x / k, i.y / k, i.z / k);
    }

    public static Vec3 operator +(Vec3 i, float k) {
      return new Vec3(i.x + k, i.y + k, i.z + k);
    }

    public static Vec3 operator -(Vec3 i, float k) {
      return new Vec3(i.x - k, i.y - k, i.z - k);
    }

    public static bool operator ==(Vec3 i, Vec3 j) {
      return Equals(i, j);
    }

    public static bool operator !=(Vec3 i, Vec3 j) {
      return !Equals(i, j);
    }

    public override string ToString() {
      return "(x: " + x + ", y: " + y + ", z: " + z + ")";
    }

    public void Set(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
    }

    public float MagnitudeSqrt() {
      return (x * x) + (y * y) + (z * z);
    }

    public float Magnitude() {
      return MathF.Sqrt(this.MagnitudeSqrt());
    }

    public Vec3 Normalize() {
      return this / this.Magnitude();
    }

    public float Dot(Vec3 other) {
      return (this.x * other.x) + (this.y * other.y) + (this.z * other.z);
    }

    public float Angle(Vec3 other) {
      var m = MathF.Abs(this.Dot(other));
      var d = this.Magnitude() * other.Magnitude();
      return MathF.Acos(m / d);
    }

    public Vec3 Cross(Vec3 other) {
      Vec3 res = new Vec3(0);
      res.x = y * other.z - z * other.y;
      res.y = z * other.x - x * other.z;
      res.z = x * other.y - y * other.x;
      return res;
    }

    public Vec3 RGB() {
      return new Vec3(x / 255.0f, y / 255.0f, z / 255.0f);
    }

    public float DistanceSqrt(Vec3 other) {
      return MathF.Pow(other.x - this.x, 2) + MathF.Pow(other.y - this.y, 2) + MathF.Pow(other.z - this.z, 2);
    }

    public float Distance(Vec3 other) {
      return MathF.Sqrt(this.DistanceSqrt(other));
    }
  }

  public class Vec4 {
    public float x;
    public float y;
    public float z;
    public float w;

    public static Vec4 zero() { return new Vec4(0); }
    public static Vec4 one() { return new Vec4(1); }

    public Vec4(float n) {
      this.x = n;
      this.y = n;
      this.z = n;
      this.w = n;
    }

    public Vec4(float x, float y, float z, float w) {
      this.x = x;
      this.y = y;
      this.z = z;
      this.w = w;
    }

    public Vec4(Vec2 v, float z, float w) {
      this.x = v.x;
      this.y = v.y;
      this.z = z;
      this.w = w;
    }

    public Vec4(Vec3 v, float w) {
      this.x = v.x;
      this.y = v.y;
      this.z = v.z;
      this.w = w;
    }

    public Vec4(Quat q) {
      this.x = q.x;
      this.y = q.y;
      this.z = q.z;
      this.w = q.w;
    }

    public float this[int i] {
      get {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;
        return 0;
      }
      set {
        if (i == 0) x = value;
        else if (i == 1) y = value;
        else if (i == 2) z = value;
        else if (i == 3) w = value;
      }
    }

    public override bool Equals(object obj) {
      if (!(obj is Vec4))
        return false;
      return ((Vec4)obj).x == this.x && ((Vec4)obj).y == this.y && ((Vec4)obj).z == this.z && ((Vec4)obj).w == this.w;
    }

    public override int GetHashCode() {
      return GetHashCode();
    }

    public static Vec4 operator +(Vec4 i, Vec4 j) {
      return new Vec4(i.x + j.x, i.y + j.y, i.z + j.z, i.w + j.w);
    }

    public static Vec4 operator -(Vec4 i, Vec4 j) {
      return new Vec4(i.x - j.x, i.y - j.y, i.z - j.z, i.w - j.w);
    }

    public static Vec4 operator *(Vec4 i, float k) {
      return new Vec4(i.x * k, i.y * k, i.z * k, i.w * k);
    }

    public static Vec4 operator /(Vec4 i, float k) {
      return new Vec4(i.x / k, i.y / k, i.z / k, i.w / k);
    }

    public static Vec4 operator +(Vec4 i, float k) {
      return new Vec4(i.x + k, i.y + k, i.z + k, i.w + k);
    }

    public static Vec4 operator -(Vec4 i, float k) {
      return new Vec4(i.x - k, i.y - k, i.z - k, i.w - k);
    }

    public static bool operator ==(Vec4 i, Vec4 j) {
      return Equals(i, j);
    }

    public static bool operator !=(Vec4 i, Vec4 j) {
      return !Equals(i, j);
    }

    public override string ToString() {
      return "(x: " + x + ", y: " + y + ", z: " + z + ", w: " + w + ")";
    }

    public void Set(float x, float y, float z, float w) {
      this.x = x;
      this.y = y;
      this.z = z;
      this.w = w;
    }

    public float MagnitudeSqrt() {
      return (x * x) + (y * y) + (z * z) + (w * w);
    }

    public float Magnitude() {
      return MathF.Sqrt(this.MagnitudeSqrt());
    }

    public Vec4 Normalize() {
      return this / this.Magnitude();
    }

    public float Dot(Vec4 other) {
      return (this.x * other.x) + (this.y * other.y) + (this.z * other.z) + (this.w * other.w);
    }

    public float Angle(Vec4 other) {
      var m = MathF.Abs(this.Dot(other));
      var d = this.Magnitude() * other.Magnitude();
      return MathF.Acos(m / d);
    }

    public Vec4 RGBA() {
      return new Vec4(x / 255.0f, y / 255.0f, z / 255.0f, w / 100.0f);
    }

    public static Vec4 operator *(Vec4 v, Mat4 m) {
      Vec4 result = new Vec4(0);
      for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
          result[r] += m[r][c] * v[c];
      return result;
    }

    public float DistanceSqrt(Vec4 other) {
      return MathF.Pow(other.x - this.x, 2) + MathF.Pow(other.y - this.y, 2) + MathF.Pow(other.z - this.z, 2) + MathF.Pow(other.w - this.w, 2);
    }

    public float Distance(Vec4 other) {
      return MathF.Sqrt(this.DistanceSqrt(other));
    }
  }

  public class Mat3 {
    public Vec3 row0;
    public Vec3 row1;
    public Vec3 row2;

    public static Mat3 identity() {
      return new Mat3(1, 0, 0,
                      0, 1, 0,
                      0, 0, 1);
    }
    public static Mat3 zero() { return new Mat3(0); }
    public static Mat3 one() { return new Mat3(1); }

    public Mat3(float n) {
      row0 = new Vec3(n);
      row1 = new Vec3(n);
      row2 = new Vec3(n);
    }

    public Mat3(Vec3 row0, Vec3 row1, Vec3 row2) {
      this.row0 = row0;
      this.row1 = row1;
      this.row2 = row2;
    }

    public Mat3(float v00, float v01, float v02,
                float v03, float v04, float v05,
                float v06, float v07, float v08) {
      row0 = new Vec3(v00, v01, v02);
      row1 = new Vec3(v03, v04, v05);
      row2 = new Vec3(v06, v07, v08);
    }

    public Vec3 this[int i] {
      get {
        if (i == 0) return row0;
        if (i == 1) return row1;
        if (i == 2) return row2;
        return new Vec3(0);
      }
      set {
        if (i == 0) row0 = value;
        else if (i == 1) row1 = value;
        else if (i == 2) row2 = value;
      }
    }

    public static Mat3 operator *(Mat3 m, float k) {
      return new Mat3(m.row0 * k, m.row1 * k, m.row2 * k);
    }

    public static Mat3 operator /(Mat3 m, float k) {
      return new Mat3(m.row0 / k, m.row1 / k, m.row2 / k);
    }

    public static Mat3 operator +(Mat3 m, float k) {
      return new Mat3(m.row0 + k, m.row1 + k, m.row2 + k);
    }

    public static Mat3 operator -(Mat3 m, float k) {
      return new Mat3(m.row0 - k, m.row1 - k, m.row2 - k);
    }

    public static Mat3 operator *(Mat3 m, Mat3 other) {
      Mat3 result = new Mat3(0);
      for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
          for (int i = 0; i < 3; ++i)
            result[c][r] += m[c][i] * other[i][r];
      return result;
    }

    public float[] ToArray() {
      float[] array = new float[9];
      array[0] = row0[0];
      array[1] = row0[1];
      array[2] = row0[2];
      array[3] = row1[0];
      array[4] = row1[1];
      array[5] = row1[2];
      array[6] = row2[0];
      array[7] = row2[1];
      array[8] = row2[2];
      return array;
    }

    public override string ToString() {
      string text = "|";
      text += row0[0] + ", " + row0[1] + ", " + row0[2] + "|\n|";
      text += row1[0] + ", " + row1[1] + ", " + row1[2] + "|\n|";
      text += row2[0] + ", " + row2[1] + ", " + row2[2] + "|";
      return text;
    }
  }

  public class Mat4 {
    public Vec4 row0;
    public Vec4 row1;
    public Vec4 row2;
    public Vec4 row3;

    public static Mat4 identity() {
      return new Mat4(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
    }
    public static Mat4 zero() { return new Mat4(0); }
    public static Mat4 one() { return new Mat4(1); }

    public Mat4(float n) {
      row0 = new Vec4(n);
      row1 = new Vec4(n);
      row2 = new Vec4(n);
      row3 = new Vec4(n);
    }

    public Mat4(Vec4 row0, Vec4 row1, Vec4 row2, Vec4 row3) {
      this.row0 = row0;
      this.row1 = row1;
      this.row2 = row2;
      this.row3 = row3;
    }

    public Mat4(float v00, float v01, float v02, float v03,
                float v04, float v05, float v06, float v07,
                float v08, float v09, float v10, float v11,
                float v12, float v13, float v14, float v15) {
      row0 = new Vec4(v00, v01, v02, v03);
      row1 = new Vec4(v04, v05, v06, v07);
      row2 = new Vec4(v08, v09, v10, v11);
      row3 = new Vec4(v12, v13, v14, v15);
    }

    public Vec4 this[int i] {
      get {
        if (i == 0) return row0;
        if (i == 1) return row1;
        if (i == 2) return row2;
        if (i == 3) return row3;
        return new Vec4(0);
      }
      set {
        if (i == 0) row0 = value;
        else if (i == 1) row1 = value;
        else if (i == 2) row2 = value;
        else if (i == 3) row3 = value;
      }
    }

    public static Mat4 operator *(Mat4 m, float k) {
      return new Mat4(m.row0 * k, m.row1 * k, m.row2 * k, m.row3 * k);
    }

    public static Mat4 operator /(Mat4 m, float k) {
      return new Mat4(m.row0 / k, m.row1 / k, m.row2 / k, m.row3 / k);
    }

    public static Mat4 operator +(Mat4 m, float k) {
      return new Mat4(m.row0 + k, m.row1 + k, m.row2 + k, m.row3 + k);
    }

    public static Mat4 operator -(Mat4 m, float k) {
      return new Mat4(m.row0 - k, m.row1 - k, m.row2 - k, m.row3 - k);
    }

    public static Mat4 operator *(Mat4 m, Mat4 other) {
      Mat4 result = new Mat4(0);
      for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
          for (int i = 0; i < 4; ++i)
            result[c][r] += m[c][i] * other[i][r];
      return result;
    }

    public override string ToString() {
      string text = "|";
      text += row0[0] + ", " + row0[1] + ", " + row0[2] + ", " + row0[3] + "|\n|";
      text += row1[0] + ", " + row1[1] + ", " + row1[2] + ", " + row1[3] + "|\n|";
      text += row2[0] + ", " + row2[1] + ", " + row2[2] + ", " + row2[3] + "|\n|";
      text += row3[0] + ", " + row3[1] + ", " + row3[2] + ", " + row3[3] + "|";
      return text;
    }

    public static Mat4 Scale(Vec3 elements) {
      Mat4 result = new Mat4(0.0f);
      result[0][0] = elements.x;
      result[1][1] = elements.y;
      result[2][2] = elements.z;
      result[3][3] = 1;
      return result;
    }

    public static Mat4 LookAt(Vec3 eye, Vec3 at, Vec3 up) {
      Vec3 f = (at - eye).Normalize();
      Vec3 s = f.Cross(up).Normalize();
      Vec3 u = s.Cross(f).Normalize();

      return new Mat4(s.x,  s.y,  s.z, -s.Dot(eye),
                  u.x,  u.y,  u.z, -u.Dot(eye),
                 -f.x, -f.y, -f.z,  f.Dot(eye),
                    0,    0,    0,  1);
    }

    public static Mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
      Mat4 result = Mat4.identity();
      result[0][0] =  2.0F / (right - left);
      result[1][1] =  2.0F / (top - bottom);
      result[2][2] = -2.0F / (zFar - zNear);
      result[3][0] = -(right + left) / (right - left);
      result[3][1] = -(top + bottom) / (top - bottom);
      result[3][2] = -(zFar + zNear) / (zFar - zNear);
      result[3][3] = 1.0f;
      return result;
    }

    public static Mat4 Perspective(float fov, float aspect, float zNear, float zFar) {
      float tFov = MathF.Tan(fov / 2.0f);
      Mat4 result = new Mat4(0.0f);
      result[0][0] = 1.0f / (aspect * tFov);
      result[1][1] = 1.0f / (tFov);
      result[2][2] = -(zFar + zNear) / (zFar - zNear);
      result[2][3] = -1.0f;
      result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
      return result;
    }

    public static Mat4 RotateX(float angle) {
      Mat4 result = Mat4.identity();
      result[1][1] = MathF.Cos(angle);
      result[1][2] = -MathF.Sin(angle);
      result[2][1] = MathF.Sin(angle);
      result[2][2] = MathF.Cos(angle);
      return result;
    }

    public static Mat4 RotateY(float angle) {
      Mat4 result = Mat4.identity();
      result[0][0] = MathF.Cos(angle);
      result[0][2] = MathF.Sin(angle);
      result[2][0] = -MathF.Sin(angle);
      result[2][2] = MathF.Cos(angle);
      return result;
    }

    public static Mat4 RotateZ(float angle) {
      Mat4 result = Mat4.identity();
      result[0][0] = MathF.Cos(angle);
      result[0][1] = -MathF.Sin(angle);
      result[1][0] = MathF.Sin(angle);
      result[1][1] = MathF.Cos(angle);
      return result;
    }

    public static Mat4 Translate(Vec3 offset) {
      Mat4 result = Mat4.identity();
      result[3] = new Vec4(offset, 1.0f);
      return result;
    }

    public float[] ToArray() {
      float[] array = new float[16];
      array[0]  = row0[0];
      array[1]  = row0[1];
      array[2]  = row0[2];
      array[3]  = row0[3];
      array[4]  = row1[0];
      array[5]  = row1[1];
      array[6]  = row1[2];
      array[7]  = row1[3];
      array[8]  = row2[0];
      array[9]  = row2[1];
      array[10] = row2[2];
      array[11] = row2[3];
      array[12] = row3[0];
      array[13] = row3[1];
      array[14] = row3[2];
      array[15] = row3[3];
      return array;
    }
  }

  public class Quat {
    public float x;
    public float y;
    public float z;
    public float w;

    public Quat(float n) {
      this.x = n;
      this.y = n;
      this.z = n;
      this.w = n;
    }

    public Quat(float x, float y, float z, float w) {
      this.x = x;
      this.y = y;
      this.z = z;
      this.w = w;
    }

    public Quat(Vec4 v) {
      this.x = v.x;
      this.y = v.y;
      this.z = v.z;
      this.w = v.w;
    }

    public Quat(Vec3 axis, float angle) {
      var s = MathF.Sin(angle / 2.0f);
      this.x = axis.x * s;
      this.y = axis.y * s;
      this.z = axis.z * s;
      this.w = MathF.Cos(angle / 2.0f);
    }

    public Quat(Mat3 m) {
      var fXSM = m[0][0] - m[1][1] - m[2][2];
      var fYSM = m[1][1] - m[0][0] - m[2][2];
      var fZSM = m[2][2] - m[0][0] - m[1][1];
      var fWSM = m[0][0] + m[1][1] + m[2][2];

      var biggestIndex = 0;
      var fBSM = fWSM;
      if (fXSM > fBSM) {
        fBSM = fXSM;
        biggestIndex = 1;
      }
      if (fYSM > fBSM) {
        fBSM = fYSM;
        biggestIndex = 2;
      }
      if (fZSM > fBSM) {
        fBSM = fZSM;
        biggestIndex = 3;
      }

      var biggestVal = MathF.Sqrt(fBSM + 1.0f) * 0.5f;
      var multiplier = 0.25f / biggestVal;

      if (biggestIndex == 0) {
        this.w = biggestVal;
        this.x = (m[1][2] - m[2][1]) * multiplier;
        this.y = (m[2][0] - m[0][2]) * multiplier;
        this.z = (m[0][1] - m[1][0]) * multiplier;
      } else if (biggestIndex == 1) {
        this.w = (m[1][2] - m[2][1]) * multiplier;
        this.x = biggestVal;
        this.y = (m[0][1] - m[1][0]) * multiplier;
        this.z = (m[2][0] - m[0][2]) * multiplier;
      } else if (biggestIndex == 2) {
        this.w = (m[2][0] - m[0][2]) * multiplier;
        this.x = (m[0][1] - m[1][0]) * multiplier;
        this.y = biggestVal;
        this.z = (m[1][2] - m[2][1]) * multiplier;
      } else if (biggestIndex == 3) {
        this.w = (m[0][1] - m[1][0]) * multiplier;
        this.x = (m[2][0] - m[0][2]) * multiplier;
        this.y = (m[1][2] - m[2][1]) * multiplier;
        this.z = biggestVal;
      }
    }

    public float this[int i] {
      get {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;
        return 0;
      }
      set {
        if (i == 0) x = value;
        else if (i == 1) y = value;
        else if (i == 2) z = value;
        else if (i == 3) w = value;
      }
    }

    public static Quat operator *(Quat q, float k) {
      return new Quat(q.x * k, q.y * k, q.z * k, q.w * k);
    }

    public static Quat operator /(Quat q, float k) {
      return new Quat(q.x / k, q.y / k, q.z / k, q.w / k);
    }

    public static Quat operator +(Quat q, float k) {
      return new Quat(q.x + k, q.y + k, q.z + k, q.w + k);
    }

    public static Quat operator -(Quat q, float k) {
      return new Quat(q.x - k, q.y - k, q.z - k, q.w - k);
    }

    public static Quat operator *(Quat p, Quat q) {
      Quat result = new Quat(0);
      result.w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
      result.x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
      result.y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
      result.z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
      return result;
    }

    public static Vec3 operator *(Quat quat, Vec3 v) {
      var QuatVector = new Vec3(quat.x, quat.y, quat.z);
      var uv = QuatVector.Cross(v);
      var uuv = QuatVector.Cross(uv);

      return v + ((uv * quat.w) + uuv) * 2.0f;
    }

    public override bool Equals(object obj) {
      if (!(obj is Quat))
        return false;
      return ((Quat)obj).x == this.x && ((Quat)obj).y == this.y && ((Quat)obj).z == this.z && ((Quat)obj).w == this.w;
    }

    public override int GetHashCode() {
      return GetHashCode();
    }

    public static bool operator ==(Quat i, Quat j) {
      return Equals(i, j);
    }

    public static bool operator !=(Quat i, Quat j) {
      return !Equals(i, j);
    }

    public override string ToString() {
      return "(x: " + x + ", y: " + y + ", z: " + z + ", w: " + w + ")";
    }

    public void Set(float x, float y, float z, float w) {
      this.x = x;
      this.y = y;
      this.z = z;
      this.w = w;
    }

    public Quat Conjugate() {
      Quat result = new Quat(0);
      result.x = -x;
      result.y = -y;
      result.z = -z;
      result.w = w;
      return result;
    }

    public Quat Inverse() {
      return Conjugate() / Dot(this);
    }

    public Quat Rotate(float angle, Vec3 v) {
      Quat result = new Quat(0);
      Vec3 vn = v.Normalize();
      result.x = vn.x * MathF.Sin(angle * 0.5f);
      result.y = vn.y * MathF.Sin(angle * 0.5f);
      result.z = vn.z * MathF.Sin(angle * 0.5f);
      result.w = MathF.Cos(angle * 0.5f);
      result = this * result;
      return result;
    }

    public float Roll() {
      return MathF.Atan2(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z);
    }

    public float Pitch() {
      var ry = 2.0f * (y * z + w * x);
      var rx = w * w - x * x - y * y + z * z;
      if (ry == 0.0f && rx == 0.0f)
        return 2.0f * MathF.Atan2(x, w);
      else
        return MathF.Atan2(ry, rx);
    }

    public float Yaw() {
      return MathF.Asin(MathV.Clamp(-2.0f * (x * z - w * y), -1.0f, 1.0f));
    }

    public Vec3 EulerAngles() {
      Vec3 result = new Vec3(0);
      result.x = Pitch();
      result.y = Yaw();
      result.z = Roll();
      return result;
    }

    public float Dot(Quat other) {
      return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    public Mat3 Mat3() {
      Mat3 result = new Mat3(0);
      var txx = 2.0f * x * x;
      var tyy = 2.0f * y * y;
      var tzz = 2.0f * z * z;
      var txy = 2.0f * x * y;
      var txz = 2.0f * x * z;
      var tyz = 2.0f * y * z;
      var txw = 2.0f * x * w;
      var tyw = 2.0f * y * w;
      var tzw = 2.0f * z * w;

      result[0] = new Vec3(1.0f - tyy - tzz,        txy + tzw,        txz - tyw);
      result[1] = new Vec3(       txy - tzw, 1.0f - txx - tzz,        tyz + txw);
      result[2] = new Vec3(       txz + tyw,        tyz - txw, 1.0f - txx - tyy);

      return result;
    }

    public Mat4 Mat4(Vec4 v) {
      Mat4 result = new Mat4(0);
      var temp = Mat3();

      result[0] = new Vec4(temp[0], 0.0f);
      result[1] = new Vec4(temp[1], 0.0f);
      result[2] = new Vec4(temp[2], 0.0f);
      result[3] = v;

      return result;
    }

    public Mat4 Mat4(Vec3 v) {
      return Mat4(new Vec4(v, 1.0f));
    }

    public Mat4 Mat4() {
      return Mat4(new Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
  }

  public class MathV {
    public static float Radians(float angle) {
      return (angle * MathF.PI) / 180.0f;
    }

    public static float Clamp(float v, float l, float r) {
      if (v < l)
        return l;
      else if (v > r)
        return r;
      else
        return v;
    }
  }

}
