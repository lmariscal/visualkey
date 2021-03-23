using System;

namespace VisualKey {

  public class Camera {
    public Shader shader { private get; set; }

    Vec2 lastMousePos;
    Vec3 up; // Camera up, not world up
    Mat4 view;
    bool firstMouseDelta = true;

    public Vec3 position;
    public Vec3 front;
    public Vec2 euler;

    public Camera() {
      shader = Shader.GetUberShader();

      position = new Vec3(0, 0, -3);
      front = new Vec3(0, 0, -1);
      up = Vec3.up();
      euler = new Vec2(-90, 0);
      lastMousePos = Input.mousePos;
    }

    private void CalculateView() {
      this.view = Mat4.LookAt(position, position + front, up);
    }

    private void UpdateInput() {
      float speed = 2.5f * Time.deltaTime;
      if (Key.W.IsPressed())
        this.position -= front * speed;
      if (Key.S.IsPressed())
        this.position += front * speed;
      if (Key.A.IsPressed())
        this.position += front.Cross(up).Normalize() * speed;
      if (Key.D.IsPressed())
        this.position -= front.Cross(up).Normalize() * speed;
      if (Key.E.IsPressed())
        this.position += up * speed;
      if (Key.Q.IsPressed())
        this.position -= up * speed;

      var mouseNow = Input.mousePos;
      var mouseDelta = mouseNow - lastMousePos;
      this.lastMousePos = mouseNow;

      if (firstMouseDelta) {
        firstMouseDelta = false;
        return;
      }

      mouseDelta *= 0.1f; //sensitivity

      this.euler += mouseDelta;
      if (this.euler.y > 89.0)
        this.euler.y = 89.0f;
      if (this.euler.y < -89.0)
        this.euler.y = -89.0f;

      this.front.x = (float)Math.Cos(MathV.Radians(euler.x)) * (float)Math.Cos(MathV.Radians(euler.y));
      this.front.y = (float)Math.Sin(MathV.Radians(euler.y));
      this.front.z = (float)Math.Sin(MathV.Radians(euler.x)) * (float)Math.Cos(MathV.Radians(euler.y));

      this.front = this.front.Normalize();
    }

    public Mat4 GetView() {
      return this.view;
    }

    public void Update() {
      this.UpdateInput();
    }

    public void Draw() {
      this.CalculateView();

      shader.Draw();
      var viewLoc = shader.GetLocation("View");
      shader.SetMat4(viewLoc, view);
    }

    public static void SetNone() {
      Shader shader = Shader.GetUberShader();
      shader.Draw();
      var viewLoc = shader.GetLocation("View");
      shader.SetMat4(viewLoc, Mat4.identity());
    }

  }

}
