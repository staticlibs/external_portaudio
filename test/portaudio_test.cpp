/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   portaudio_test.cpp
 * Author: alex
 *
 * Created on June 24, 2015, 10:38 AM
 */

#include <iostream>
#include <string>
#include <thread>
#include <sstream>
#include <limits>
#include <chrono>
#include <cassert>
#include <cstring>

#include "portaudio.h"

// http://portaudio.com/docs/v19-doxydocs/tutorial_start.html

/* This routine will be called by the PortAudio engine when audio is needed.
It may called at interrupt level on some machines so don't do anything
that could mess up the system like calling malloc() or free().
 */
static int patestCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
    (void) inputBuffer;
    (void) outputBuffer;
    (void) framesPerBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;
    static float max = std::numeric_limits<float>::min();
    static float min = std::numeric_limits<float>::max();

    const float* st = static_cast<const float*> (inputBuffer);
    for (unsigned int i = 0; i < framesPerBuffer; i+=4) {
        float val = *(st + i);
        if (val > max) {
            max = val;
        }
        if (val < min) {
            min = val;
        }
    }
    std::cout << min << " : " << max << "\n" << std::endl;    
    return PaStreamCallbackResult::paContinue;
}

int main() {
    // init
    auto errinit = Pa_Initialize();
    (void) errinit;
    assert(errinit == paNoError);
    
    auto numDevices = Pa_GetDeviceCount();
    std::cout << "----------------" << std::endl;
//    std::cout << numDevices << std::endl;
    int defnum = 0;
    for (int i = 0; i < numDevices; i++) {
        auto deviceInfo = Pa_GetDeviceInfo(i);
        //std::cout << deviceInfo->name << std::endl;
        std::string defname{"sysdefault"};
        if (defname == deviceInfo->name) {
            defnum = i;
        }
    }
    std::cout << defnum << std::endl;
    
    PaStream* stream;
    PaStreamParameters inputParameters;
    memset(&inputParameters, 0, sizeof(PaStreamParameters));
    inputParameters.channelCount = 1;
    inputParameters.device = defnum;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(defnum)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field
    auto erropen = Pa_OpenStream(
            &stream,
            &inputParameters,
            nullptr,
            44100,
            paFramesPerBufferUnspecified,
            paNoFlag, //flags that can be used to define dither, clip settings and more
            patestCallback, //your callback function
            nullptr); //data to be passed to callback. In C++, it is frequently (void *)this
    assert(erropen == paNoError);
    auto errstart = Pa_StartStream(stream);
    assert(errstart == paNoError);
    
    std::this_thread::sleep_for(std::chrono::seconds{5});
    
    auto errstop = Pa_StopStream(stream);
    assert(errstop == paNoError);
    
    auto errclose = Pa_CloseStream(stream);
    assert(errclose == paNoError);
    std::cout << "----------------" << std::endl;
    
    // terminate
    auto errterm = Pa_Terminate();
    (void) errterm;
    assert(errterm == paNoError);

    std::cout << "test ran" << std::endl;
    return 0;
}

