//
//  DGWavetableSynth.h
//  DINBlossom
//
//  Created by Karl Scholz on 7/7/17.
//  Copyright © 2017 Data Garden
//

/*

 Wavetable synths require some Objective C code to utilize Novocaine's AudioFileLoader
 
 The idea here is to have methods that populate a C struct that 
 is then used similarly to the modules found in BSCommonModules.h
 
 The format of these C modules follows the convention laid out in BSCommonModules.h:
 
 A C Struct is used for storing state,
 an init method sets up the state, 
 and a do method is called from within the audio render callback.
 
 Wavetable synths are themselves polyphonic, holding an array of DGSamples
 
*/
#ifndef DGWavetableSynth_h
#define DGWavetableSynth_h
//#import "BSSingleton.h"
#import "BSCommonModules.h"
#import "AudioFile.h"
#import "json.hpp"
#import "InterpPack.h"
#import "ResamplerFlt.h"
#include <string>

typedef struct {
    DGSample * sample;
    int        lowNote;
    int        highNote;
}DGSampleRegion;

typedef struct {
    DGSample * sample;
    float      note;
    float      rootNote;
    float      read_index_increment;//tuned jump per sample
    float      read_index;
    float      pan[2];//level for L and R
    float      gain;
    float      tuningFrequency;
    bool       playing;
    
    long      read_index_literal;//tracking how many samples have been rendered total
    long      pitch_rspl;
    
    rspl::ResamplerFlt resampler_flt;
}DGSampleVoice;

typedef struct{
    //array of sample regions
    DGSampleRegion * regions;
    int              region_count;
    int polyphony;
    int voice_index_to_trigger;
    //array of voices that actually read the samples
    DGSampleVoice * voices;
    
    int synthId;
    
    bool loaded;
    bool regionsSet;
        
}DGWavetableSynth;


void DGSampleVoice_init(DGSampleVoice * voice, DGSample * sample, int note);
void DGSampleVoice_do(DGSampleVoice * voice, float * output);

void DGWavetableSynth_playNote(DGWavetableSynth * synth, int note);
void DGWavetableSynth_init(DGWavetableSynth * synth);
void DGWavetableSynth_clear(DGWavetableSynth * synth);
void DGWavetableSynth_do(DGWavetableSynth * synth, float * output);
void DGWavetableSynth_setTuningFrequency(DGWavetableSynth * synth, float frequency);

void DGSampleRegion_clear(DGSampleRegion * region);

void DGSampleVoice_updateTuningFrequency(DGSampleVoice * voice);
void DGSampleVoice_clear(DGSampleVoice * voice);

class DGWavetableSynthManager{
public:
    DGWavetableSynthManager(std::map<std::string, std::vector<std::string>>paths);
    ~DGWavetableSynthManager();
    DGWavetableSynth * newSynthWithSamples(nlohmann::json audiofilejson);
    void loadSampleIntoSynth(DGWavetableSynth * synth, nlohmann::json audiofilejson);
    std::map<std::string, std::vector<std::string>>samplePaths;

private:
    
};
    
#endif
