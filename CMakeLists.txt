# Copyright 2015, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_gcc CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_portaudio C CXX )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 2.0 )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/staticlibs_common.cmake )

if ( STATICLIB_TOOLCHAIN MATCHES "(linux|macosx)_[^_]+_[^_]+" )
    # non-windows indirect build
    # variabled for makefiles
    if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
        set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
    #    set ( ${PROJECT_NAME}_CXXFLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}" )
        set ( ${PROJECT_NAME}_DEBUG "--enable-debug-output" )
    endif ( )
    if ( CMAKE_BUILD_TYPE MATCHES "Release" )
        set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
    #    set ( ${PROJECT_NAME}_CXXFLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" )
        set ( ${PROJECT_NAME}_DEBUG "--disable-debug-output" )
    endif ( )
    if ( STATICLIB_TOOLCHAIN MATCHES "linux_[^_]+_[^_]+" )
        set ( ${PROJECT_NAME}_WITH_BACKEND "--with-alsa" )
    endif ( )

    # library config
    configure_file ( ${CMAKE_CURRENT_LIST_DIR}/Makefile-${STATICLIB_TOOLCHAIN}
            ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
    add_custom_target( ${PROJECT_NAME}_build
            COMMAND ${CMAKE_MAKE_PROGRAM} -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
    add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
    set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION 
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libportaudio${CMAKE_STATIC_LIBRARY_SUFFIX} )
    add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_build )
    set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/portaudio/include )

# windows direct CMake build, based on https://subversion.assembla.com/svn/portaudio/portaudio/trunk/CMakeLists.txt
elseif ( STATICLIB_TOOLCHAIN MATCHES "windows_[^_]+_msvc" )

    set ( GENERATED_MESSAGE "CMake generated file, do NOT edit! Use CMake-GUI to change configuration instead." )
    option ( PA_USE_WMME "WMME backend" ON )
    configure_file ( portaudio/cmake_support/options_cmake.h.in ${CMAKE_CURRENT_BINARY_DIR}/portaudio/options_cmake.h @ONLY )

    add_library ( ${PROJECT_NAME} STATIC
            # WMME
            portaudio/src/hostapi/wmme/pa_win_wmme.c
            # common
            portaudio/src/common/pa_allocation.c
            portaudio/src/common/pa_converters.c
            portaudio/src/common/pa_cpuload.c
            portaudio/src/common/pa_debugprint.c
            portaudio/src/common/pa_dither.c
            portaudio/src/common/pa_front.c
            portaudio/src/common/pa_process.c
            portaudio/src/common/pa_ringbuffer.c
            portaudio/src/common/pa_stream.c
            portaudio/src/common/pa_trace.c
            # platform
            portaudio/src/os/win/pa_win_hostapis.c
            portaudio/src/os/win/pa_win_util.c
            portaudio/src/os/win/pa_win_waveformat.c
            portaudio/src/os/win/pa_win_wdmks_utils.c
            portaudio/src/os/win/pa_win_coinitialize.c
            portaudio/src/os/win/pa_x86_plain_converters.c
    )

    set ( ${PROJECT_NAME}_INCLUDES 
            portaudio/src/os/win
            portaudio/include
            portaudio/src/common
            ${CMAKE_CURRENT_BINARY_DIR}/portaudio
    )
    target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )

    target_compile_definitions ( ${PROJECT_NAME} PRIVATE
            -DUNICODE
            -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DPORTAUDIO_CMAKE_GENERATED
    )

    if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
        target_compile_definitions ( ${PROJECT_NAME} PRIVATE -DPA_ENABLE_DEBUG_OUTPUT)
    endif ( )


endif ( )

# tests
if ( "${STATICLIB_ROOT_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}" )
    enable_testing ( )
    set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
    add_executable ( portaudio_test ${CMAKE_CURRENT_LIST_DIR}/test/portaudio_test.cpp )
    target_include_directories ( portaudio_test BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
    target_link_libraries ( portaudio_test ${PROJECT_NAME} )
    if ( STATICLIB_TOOLCHAIN MATCHES "linux_armv6rpi_gcc" )
        # libasound2-dev_1.0.25-4_armhf.deb -> SYSROOT/usr/include
        # libasound2_1.0.25-4_armhf.deb -> SYSROOT/usr/lib/libasound.so
        target_link_libraries ( portaudio_test rt pthread asound )
    elseif ( STATICLIB_TOOLCHAIN MATCHES "linux_[^_]+_[^_]+" )
        target_link_libraries ( portaudio_test rt asound )
    endif ( )
    # http://stackoverflow.com/q/27998009/314015
#    if ( DEFINED CMAKE_MEMORYCHECK_COMMAND )
#        add_test ( portaudio_test 
#                ${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS} 
#                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/portaudio_test )
#    else ( )
    add_test ( portaudio_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/portaudio_test )
#    endif ( )
endif ( )

# headers export
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
