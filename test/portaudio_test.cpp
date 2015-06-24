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
#include <cassert>

#include "portaudio.h"

// http://portaudio.com/docs/v19-doxydocs/tutorial_start.html
//typedef struct {
//    float left_phase;
//    float right_phase;
//}
//paTestData;
//
///* This routine will be called by the PortAudio engine when audio is needed.
//It may called at interrupt level on some machines so don't do anything
//that could mess up the system like calling malloc() or free().
// */
//static int patestCallback(const void *inputBuffer, void *outputBuffer,
//        unsigned long framesPerBuffer,
//        const PaStreamCallbackTimeInfo* timeInfo,
//        PaStreamCallbackFlags statusFlags,
//        void *userData) {
//    /* Cast data passed through stream to our structure. */
//    paTestData *data = (paTestData*) userData;
//    float *out = (float*) outputBuffer;
//    unsigned int i;
//    (void) inputBuffer; /* Prevent unused variable warning. */
//    for (i = 0; i < framesPerBuffer; i++) {
//        *out++ = data->left_phase; /* left */
//        *out++ = data->right_phase; /* right */
//        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
//        data->left_phase += 0.01f;
//        /* When signal reaches top, drop back down. */
//        if (data->left_phase >= 1.0f) data->left_phase -= 2.0f;
//        /* higher pitch so we can distinguish left and right. */
//        data->right_phase += 0.03f;
//        if (data->right_phase >= 1.0f) data->right_phase -= 2.0f;
//    }
//    return 0;
//}

int main() {
    auto errinit = Pa_Initialize();
    assert(errinit == paNoError);
    auto errterm = Pa_Terminate();
    assert(errterm == paNoError);

    std::cout << "test ran" << std::endl;
    return 0;
}

