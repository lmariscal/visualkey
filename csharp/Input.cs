using System.Runtime.CompilerServices;

namespace VisualKey {

  public enum MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2,
    Button3 = 3,
    Button4 = 4,
    Button5 = 5,
    Button6 = 6,
    Button7 = 7,
    Button8 = 8,
    Last
  }

  public enum Pad {
    Pad0 = 0,
    Pad1 = 1,
    Pad2 = 2,
    Pad3 = 3,
    Pad4 = 4,
    Pad5 = 5,
    Pad6 = 6,
    Pad7 = 7,
    Pad8 = 8,
    Pad9 = 9,
    Pad10 = 10,
    Pad11 = 11,
    Pad12 = 12,
    Pad13 = 13,
    Pad14 = 14,
    Pad15 = 15,
    Last
  }

  public enum JoyStick {
    LeftX = 0,
    LeftY = 1,
    RightX = 2,
    RightY = 3,
    LeftBumper = 4,
    RightBumper = 5,
    Last
  }

  public enum PadButton {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftBumper = 4,
    RightBumper = 5,
    Back = 6,
    Start = 7,
    Guide = 8,
    LeftThumb = 9,
    RightThumb = 10,
    DPadUp = 11,
    DPadRight = 12,
    DPadDown = 13,
    DPadLeft = 14,
    Last
  }

  public enum Key {
    Unknown = -1,
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    N0 = 48,
    N1 = 49,
    N2 = 50,
    N3 = 51,
    N4 = 52,
    N5 = 53,
    N6 = 54,
    N7 = 55,
    N8 = 56,
    N9 = 57,
    Semicolon = 59,
    Equal = 61,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,
    World1 = 161,
    World2 = 162,
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    KP0 = 320,
    KP1 = 321,
    KP2 = 322,
    KP3 = 323,
    KP4 = 324,
    KP5 = 325,
    KP6 = 326,
    KP7 = 327,
    KP8 = 328,
    KP9 = 329,
    KPDecimal = 330,
    KPDivide = 331,
    KPMultiply = 332,
    KPSubtract = 333,
    KPAdd = 334,
    KPEnter = 335,
    KPEqual = 336,
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,
    Last
  }

  public static class Input {
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsKeyPressed(int key);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsMousePressed(int button);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsPadPressed(int pad, int button);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsKeyJustPressed(int key);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsMouseJustPressed(int button);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsPadJustPressed(int pad, int button);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsKeyJustReleased(int key);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsMouseJustReleased(int button);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static bool IsPadJustReleased(int pad, int button);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static float GetJoyStick(int joyStick);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static Vec2 GetMousePos();

    public static Vec2 mousePos {
      get { return GetMousePos(); }
      set { }
    }

    public static bool IsPressed(this Key key) {
      return IsKeyPressed((int)key);
    }

    public static bool IsPressed(this MouseButton button) {
      return IsMousePressed((int)button);
    }

    public static bool IsPressed(this Pad pad, PadButton button) {
      return IsPadPressed((int)pad, (int)button);
    }

    public static bool IsJustPressed(this Key key) {
      return IsKeyJustPressed((int)key);
    }

    public static bool IsJustPressed(this MouseButton button) {
      return IsMouseJustPressed((int)button);
    }

    public static bool IsJustPressed(this Pad pad, PadButton button) {
      return IsPadJustPressed((int)pad, (int)button);
    }

    public static bool IsJustReleased(this Key key) {
      return IsKeyJustReleased((int)key);
    }

    public static bool IsJustReleased(this MouseButton button) {
      return IsMouseJustReleased((int)button);
    }

    public static bool IsJustReleased(this Pad pad, PadButton button) {
      return IsPadJustReleased((int)pad, (int)button);
    }

    public static float GetAxis(this JoyStick joyStick) {
      return GetJoyStick((int)joyStick);
    }
  }

}
