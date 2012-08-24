How to Compile the Demo
----------------------

This demo uses CMAKE as it's build system.
MACOSX
------

On MACOSX should be enough run the commands:
  cmake .
  make
And run the executable called "Demo" with:

  ./Demo

WINDOWS
-------
On Windows CMAKE-gui can be used to build the system using the Visual Studio 
generator. I tested the build on Windows 7 with VS2010 using CMAKE 2.8.9 and 
it worked well. 
IMPORTANT: To Run the demo is necessary to start it from the 
"Debug" or "Release" directory (depending on the configuration for which 
the executable has been built, because there are hardcoded directory references
in the main_win32.cpp code. Running it directly from Visual Studio brings 
a black window , because the models are not loaded)

LINUX
-----
On Linux can be used the same commands of the MACOSX version, but the library 
GLFW must be compiled and the static library file of this library 
(called "libglfw.a") must be copied in the root directory.
Building on Linux of this hasn't been tested, but it SHOULD work.