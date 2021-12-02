//
//  DGOscillatorSynth.hpp
//  DGEngine
//
//  Created by Karl Scholz on 12/8/18.
//

#ifndef DGOscillatorSynth_h
#define DGOscillatorSynth_h

#include <stdio.h>
#include "BSCommonModules.h"

typedef struct{
    DGBasicOscillator * oscillator;
    float rootFrequency;
    float ramp;
    float ramp_depth;
    float ramp_rate;
}DGOscillatorSynth;

void DGOscillatorSynth_playNote(DGOscillatorSynth * synth, int note);
void DGOscillatorSynth_init(DGOscillatorSynth * synth, float sample_rate);
void DGOscillatorSynth_clear(DGOscillatorSynth * synth);
void DGOscillatorSynth_do(DGOscillatorSynth * synth, float * output);
void DGOscillatorSynth_setTuningFrequency(DGOscillatorSynth * synth, float frequency);

#endif /* DGOscillatorSynth_h */
