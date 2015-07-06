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

Staticlib toolchain name must be specified as a `STATICLIB_TOOLCHAIN` parameter to `cmake` command
unless you are using GCC on Linux x86_64 (`linux_amd64_gcc` toolchain) that is default.

Example build for Windows x86_64 with Visual Studio 2013 Express, run the following commands 
from the development shell `C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x64 Cross Tools Command Prompt` :

    git clone https://github.com/staticlibs/external_portaudio.git
    cd external_portaudio
    git submodule update --init --recursive
    mkdir build
    cd build
    cmake .. -DSTATICLIB_TOOLCHAIN=windows_amd64_msvc -G "NMake Makefiles"
    nmake
    nmake test

`libasound2-dev` is required for Linux builds.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

Changelog
---------

**2015-07-06**

 * version 2.0.0 initial build
