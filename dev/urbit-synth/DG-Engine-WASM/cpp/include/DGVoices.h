//
//  DGVoiceManager.h
//  DINBlossom
//
//  Created by Karl Scholz on 7/7/17.
//  Copyright © 2017 Data Garden
//

/*

 DGVoices.h defines the individual voice routing for each voice in a given DataGarden Sound Engine
 
 ChannelStrips are just a struct with pointers to each module in the strip.
 The init and do methods follow the same conventions as BSCommonModules.h, DGWavetableSynth.h:
 
 A struct holds the state
 An init method sets up the state
 A do method updates the state and prints to a sample buffer
 
*/

#ifndef DGVoices_h
#define DGVoices_h

#define kMaxNumberOfVoices 4
/////////////////////////////////////////////

/////////////////////////////////////////////

#import "BSCommonModules.h"
#import "DGWavetableSynth.h"
#import "DGArpeggiator.h"
#import "DGTuningParameters.h"
#import "gverb.h"
#import "gverbdsp.h"
#import "json.hpp"
#import "InterpPack.h"


typedef enum{
    
    BSParameterRangeMapDestination_Voice1Gain,
    BSParameterRangeMapDestination_Voice2Gain,
    BSParameterRangeMapDestination_Voice3Gain,
    BSParameterRangeMapDestination_Voice4Gain,
    
    BSParameterRangeMapDestination_Voice1ArpSubdivision,
    BSParameterRangeMapDestination_Voice2ArpSubdivision,
    BSParameterRangeMapDestination_Voice3ArpSubdivision,
    BSParameterRangeMapDestination_Voice4ArpSubdivision,

    BSParameterRangeMapDestination_Voice1EffectsSendA,
    BSParameterRangeMapDestination_Voice1EffectsSendB,
    
    BSParameterRangeMapDestination_Voice2EffectsSendA,
    BSParameterRangeMapDestination_Voice2EffectsSendB,
    
    BSParameterRangeMapDestination_Voice3EffectsSendA,
    BSParameterRangeMapDestination_Voice3EffectsSendB,
    
    BSParameterRangeMapDestination_Voice4EffectsSendA,
    BSParameterRangeMapDestination_Voice4EffectsSendB,
    
    BSParameterRangeMapDestination_MasterEffectsSendA,
    BSParameterRangeMapDestination_MasterEffectsSendB,
    
}BSParameterRangeMapDestination;

#define kNumberOfRangeMaps 18


typedef struct{
    DGArpeggiator * arpeggiator;
    DGWavetableSynth * synth;
    DGDelay * delay;
    BSFilter * lp_filter;
    BSFilter * hp_filter;
    DGResonator * delayResonator;
    DGVCA * outputVCA;
    int voiceNumber;
    bool active;
    
    //corellates to device multimode id number
    int multimodeID;
}DGVoice;
void DGVoice_init(DGVoice * voice, float sample_rate, DGEngineState * state, int voiceNumber);
void DGVoice_do(DGVoice * voice, float * input, float * output, DGEngineState * state);

/* Deprecated Voice1

#pragma mark - DGVoice1 definition
typedef struct{
    DGArpeggiator * arpeggiator;
    DGWavetableSynth * synth;
    DGVCA * outputVCA;

    bool active;
    
}DGVoice1;
void DGVoice1_init(DGVoice1 * voice, float sample_rate, DGEngineState * state);
void DGVoice1_do(DGVoice1 * voice, float * input, float * output, DGEngineState * state);
*/
#pragma mark - DGVoice1Effects definition
typedef struct{
    DGVCA * inputVCA;
    BSFilter * filter;
    DGBitCrusher * crusher;
    DGResonator * resonator;
}DGVoice1Effects;
void DGVoice1Effects_init(DGVoice1Effects * voice1_effects, float sample_rate);
void DGVoice1Effects_clear(DGVoice1Effects * voice1_effects);
void DGVoice1Effects_do(DGVoice1Effects * voice1_effects, float * input, float * output);


#pragma mark - BusA definition
typedef struct{
    float filterFreq;
    DGVCA * freqVCA;
    BSFilter * hp_filter;
    BSFilter * lp_filter;
}DGBusA;
void DGBusA_init(DGBusA * voice, float sample_rate);
void DGBusA_do(DGBusA * voice, float * input, float * output);

#pragma mark - BusB definition
typedef struct{
    float filterFreq;
    DGVCA * freqVCA;
    BSFilter * hp_filter;
    BSFilter * lp_filter;
}DGBusB;
void DGBusB_init(DGBusB * voice, float sample_rate);
void DGBusB_do(DGBusB * voice, float * input, float * output);

#pragma mark - DGOutputEffects definition
typedef struct{
    DGVCA * resonatorVCA;
    DGResonator * resonator;
    DGDelay * delay;
    BSFilter * filter;
    DGVCA * delayTimeVCA;
    DGModDelay * modDelay;
    
    //non modulating always on reverb on 'dry' signal
    ty_gverb * static_reverb[2];
    
    DGVCA * reverbDryVCA;
    DGVCA * reverbSendVCA;
    
    bool reverbSet;
    int reverbActiveFrameCounter;

    ty_gverb * reverb[2];
    BSFilter * output_hp_filter;
}DGOutputEffects;
void DGOutputEffects_init(DGOutputEffects * output_effects, float sample_rate);
void DGOutputEffects_clear(DGOutputEffects * output_effects);
void DGOutputEffects_updateReverb(DGOutputEffects * output_effects, nlohmann::json reverbJSON, float sample_rate);

void DGOutputEffects_do(DGOutputEffects * output_effects, float * input, float * output, DGEngineState * state);

/*
#pragma mark - DGVoice initialization methods
//Initialization
@interface DGVoiceManager : BSSingleton
+(DGVoice1*)newVoice1WithSampleRate:(float)sampleRate;
+(DGVoice1Effects*)newVoice1EffectsWithSampleRate:(float)sampleRate;
+(DGVoice2*)newVoice2WithSampleRate:(float)sampleRate;
+(DGVoice3*)newVoice3WithSampleRate:(float)sampleRate;
+(DGOutputEffects*)newOutputEffectsWithSampleRate:(float)sampleRate;
@end
*/


#pragma mark - DGAudioGraph
typedef struct{

    DGEngineState * state;
    
    //DGVoice * voices;

    DGVCA * voice1SendAVCA;
    DGVCA * voice1SendBVCA;
    
    int numberOfVoices;
    DGVoice * voices[kMaxNumberOfVoices];
    
    DGBusA   * busA;
    DGBusB   * busB;
    
    DGOutputEffects * outputEffects;
    DGModDelay * modDelay;
    
    BSParameterRangeMap rangeMaps[kNumberOfRangeMaps];
    
    float beatstamp;
    
    float sampleRate;
    
    bool playbackEnabled;
    bool muted;
    
    //int ids stored for multimode assignment - duplicates ok
    bool multimode_enabled;
    int multimodeAssignments[kMaxNumberOfVoices];
    
    std::map<std::string, std::vector<std::string>> samplePaths;
    
}DGAudioGraph;

void DGAudioGraph_init(DGAudioGraph * graph, float sample_rate, nlohmann::json json_state, std::map<std::string, std::vector<std::string>> samplePaths);
void DGAudioGraph_do(DGAudioGraph * graph, float * input, float * output);
void DGAudioGraph_midi(DGAudioGraph * graph, BSMidiMessage * m);
void DGAudioGraph_setTuningFrequency(DGAudioGraph* graph, float frequency);
void DGAudioGraph_incrementRootKey(DGAudioGraph* graph, int direction);
void DGAudioGraph_incrementScale(DGAudioGraph* graph, int direction);
void DGAudioGraph_setKeyScale(DGAudioGraph* graph, BSRootKey key, BSMusicalScale scale);
void DGAudioGraph_clear(DGAudioGraph* graph);
void DGAudioGraph_enableMultimode(DGAudioGraph * graph, bool enable);
void DGAudioGraph_setMultimodeIDForVoice(DGAudioGraph * graph, int multimode_id, int voice);
int  DGAudioGraph_getMultimodeIDForVoice(DGAudioGraph * graph, int voice);

//For assiting the editor: must be called after DGAudioGraph_init
void DGAudioGraph_updateRealtimeParameters(DGAudioGraph * graph, float sample_rate, nlohmann::json json_state);
void DGAudioGraph_updateRealtimeParametersReverb(DGAudioGraph * graph, float sample_rate, nlohmann::json json_state);

void DGAudioGraph_reloadVoice(DGAudioGraph * graph, float sample_rate, int voiceNumber, nlohmann::json json_state);

void loadStateFromJSON(nlohmann::json json_data, DGEngineState * state);

class DGVoiceManager{
public:
    DGVoiceManager();
    ~DGVoiceManager();
    DGVoice * newVoiceWithSampleRate(float sampleRate, DGAudioGraph * graph, int voiceNumber);
    DGOutputEffects * newOutputEffectsWithSampleRate(float sampleRate);
private:
    
};

#endif
