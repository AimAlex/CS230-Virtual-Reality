## Assignment #1

### Environment

* OpenGL 4.1
* GLSL 410

### Requirements

* CMake
* Assimp
* FreeImage
* SDL2
* GLEW

### Play with teapot on Mac OS

Mac OS is a wonderful operating system, but since the OpenGL version is old (3.3 or 4.1), it doesn't like playing with teapot. The solution is as follows:

- Download the attached file and unzip it.
- Install packages: Cmake, Assimp, freeimage, SDL2, GLEW...
- Cmake the file into ./build with current generator Xcode.
- Open the xcodeproj and modify "Other Linker Flags":
  - -lassimp -> /usr/local/lib/libassimp.4.1.0.dylib,

​        -freeimage -> /usr/local/lib/libfreeimage.3.dylib

- Run it, then you can play with the teapot happily.


