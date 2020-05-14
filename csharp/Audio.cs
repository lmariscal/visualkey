using System.Runtime.CompilerServices;
using System;

namespace VisualKey {

  public class Audio {
    public string path;
    public bool loop;

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void PlayAudio(string path, bool loop);

    public Audio(string path, bool loop = false) {
      this.path = path;
      this.loop = loop;
    }

    public void Play() {
      PlayAudio(this.path, this.loop);
    }
  }

}
