using System;
using VisualKey;

class App {

  Window window = new Window(1280, 720);
  Rectangle rectangle = new Rectangle(100, 100);

  void Start() {
    Console.WriteLine("Starting...");
  }

  void Update() {
    rectangle.Draw();
  }

  void Stop() {
    Console.WriteLine("Stopping...");
  }
}
