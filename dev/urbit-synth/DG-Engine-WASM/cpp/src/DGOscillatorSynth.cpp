//
//  DGOscillatorSynth.cpp
//  DGEngine
//
//  Created by Karl Scholz on 12/8/18.
//

#include "DGOscillatorSynth.h"
#include <iostream>

void DGOscillatorSynth_init(DGOscillatorSynth * synth, float sample_rate){
    
    synth->ramp = 0.0f;
    synth->ramp_rate = 1.0f / sample_rate;
    synth->ramp_depth = 2000.0f;
    
    synth->oscillator = new DGBasicOscillator;
    DGBasicOscillator_init(synth->oscillator, sample_rate, 100);
}
void DGOscillatorSynth_clear(DGOscillatorSynth * synth){
    
    //DGBasicOscillator_clear(synth->oscillator);
    delete synth->oscillator;
}


void DGOscillatorSynth_playNote(DGOscillatorSynth * synth, int note){
    
    //reset
    synth->rootFrequency = frequencyFromMIDINote(note);
    synth->ramp = 0;
    //synth->oscillator->frequency = frequencyFromMIDINote(note);
}

void DGOscillatorSynth_do(DGOscillatorSynth * synth, float * output){
    
    float frequency = synth->rootFrequency + synth->ramp;
    
    if(frequency < 20000 && frequency > 0)
        synth->oscillator->frequency = synth->rootFrequency + synth->ramp;
    
    DGBasicOscillator_do(synth->oscillator, output);
    synth->ramp += (synth->ramp_rate * synth->ramp_depth);
}

void DGOscillatorSynth_setTuningFrequency(DGOscillatorSynth * synth, float frequency){
    
}

