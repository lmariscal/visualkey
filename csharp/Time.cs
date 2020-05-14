using System.Runtime.CompilerServices;

namespace VisualKey {

  public class Time {
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static float DeltaTime();
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static double GetTime();

    static public float deltaTime {
      get {
        return DeltaTime();
      }
      private set { }
    }

    static public double time {
      get {
        return GetTime();
      }
      private set { }
    }
  }

}
