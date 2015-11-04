PortAudio library build for Staticlibs
======================================

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [PortAudio library](http://www.portaudio.com/).

Only the following Staticlibs platforms are supported by this build:

 - Windows x86 and x86_64 using [WMME](https://en.wikipedia.org/wiki/Windows_legacy_audio_components#Multimedia_Extensions_.28MME.29) backend
 - Linux using [ALSA](http://www.alsa-project.org/main/index.php/Main_Page) backend:
  * Linux x86_64 with GCC
  * Linux x86_64 with Clang
  * Raspbian Linux for Raspberry Pi
  * Debian Linux armhf for Cortex-A7 boards

Project uses `Makefile`s for Linux builds and native CMake for Windows builds.

[Mirror of PortAudio repository](https://github.com/staticlibs/mirror_portaudio) is used as a submodule for this project.

Link to [documentation](http://www.portaudio.com/docs/).

How to build
------------

[CMake](http://cmake.org/) is required for building.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone --recursive https://github.com/staticlibs/external_portaudio.git
    cd external_portaudio
    mkdir build
    cd build
    cmake ..
    msbuild external_portaudio.sln

To build on other platforms using GCC or Clang with GNU Make:

    cmake .. -DCMAKE_CXX_FLAGS="--std=c++11" -DSTATICLIB_TOOLCHAIN=...
    make

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

`libasound2-dev` is required for Linux builds.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2015-11-04**

 * version 2.0.1
 * `pkg-config` integration

**2015-07-06**

 * version 2.0.0 initial build
