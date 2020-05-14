## VisualKey

### How to compile?

- `mkdir bld`
- `cd bld`
- `cmake ..`
- `make`

Download IRRKlang from their [download page](https://www.ambiera.com/irrklang/downloads.html).
Extract it and copy the files inside the `irrKlang-Xbit-X.X.X` directory into `third/irrklang`.

#### Audio

It uses [IRRKlang](https://www.ambiera.com/irrklang/index.html) as a sound engine and audio library. It is free for
non-commercial use but you must buy a license for commercial projects.
You can visit the [official license page](https://www.ambiera.com/irrklang/license.html) for more information.

A pull request with another sound engine would be highly appreciated. Please note that we would be looking for a
cross platform solution and an audio library which at least supports `WAV`, `OGG`.
