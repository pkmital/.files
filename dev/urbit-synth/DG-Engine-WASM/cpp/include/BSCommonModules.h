//
//  BSCommonModules.h
//  DINBlossom
//
//  Created by Karl Scholz on 6/26/17.
//  Copyright © 2017 Data Garden
//

/*
 
BSCommonModules is a collection of audio modules that all follow the same format:
 
A C struct is used for storing state
An 'init' method sets up the state
A 'do' method updates the state and writes to a supplied buffer
 
Code contained in these modules makes up the bulk of the actual running code in our audio render callback.
 
See also the DGWavetableSynth.h, DGArpeggiator, and DGVoices.h headers for similarly structured audio modules

*/

#ifndef BSCommonModules_hpp
#define BSCommonModules_hpp

#include <stdint.h>
//#include <cstdint>
#include <math.h>
#include <stdio.h>
#import "json.hpp"
#import "MipMapFlt.h"

typedef enum {
    BSMusicalScalePentatonicMajor,
    BSMusicalScalePentatonicMinor,
    BSMusicalScaleChromatic,
    BSMusicalScaleMajor,
    BSMusicalScaleNaturalMinor,
    BSMusicalScaleHarmonicMinor,
    BSMusicalScaleIonian,//major
    BSMusicalScaleDorian,
    BSMusicalScalePhrygian,
    BSMusicalScaleLydian,
    BSMusicalScaleMixolydian,
    BSMusicalScaleAeolian,//natural minor
    BSMusicalScaleLocrian,
    BSMusicalScaleWholeTone,
    BSMusicalScaleJazzMinor,
    BSMusicalScaleHungarian,
    BSMusicalScaleCount,//this is here just to give the length of enum -- Always have this last
} BSMusicalScale;

typedef struct{
    BSMusicalScale scale;
    int notesLength;
    float * notes;
}BSMusicalScaleMap;

char* BSMusicalScale_nameForScale(BSMusicalScale scale);
BSMusicalScale BSMusicalScale_scaleFromString(char * name);
BSMusicalScaleMap * BSMusicalScale_mapForScale(BSMusicalScale scale);

typedef enum {
    BSRootKeyA,
    BSRootKeyASharp,
    BSRootKeyB,
    BSRootKeyC,
    BSRootKeyCSharp,
    BSRootKeyD,
    BSRootKeyDSharp,
    BSRootKeyE,
    BSRootKeyF,
    BSRootKeyFSharp,
    BSRootKeyG,
    BSRootKeyGSharp,
    BSRootKeyCount,
} BSRootKey;
char* BSRootKey_nameForRootKey(BSRootKey key);
int BSRootKey_midiNumberForRootKey(BSRootKey key);
BSRootKey BSRootKey_rootKeyFromString(char * name);

typedef struct {
    unsigned char message[3];
    double timestamp;//in ms - relative to current app time
    float beatstamp; //timestamp relative to song position 1.0 is on first beat
    bool scheduled;  //used for messages that are scheduled in advance - see DGAudioEngineMidiScheduler
    int deviceMultimodeID;
} BSMidiMessage;

typedef struct {
    BSMidiMessage * messages;
    int length;
} BSMidiMessageArray;

typedef struct{
    //float *            samples;
    uint64_t           length;
    float              rootNote;
    rspl::MipMapFlt    mip_map;
    std::vector<float> samples;
}DGSample;

void DGSample_clear(DGSample * sample);

void BSMakePanGains(float angle, float * gains);

#pragma mark - Frequency and Sample Size Math

float frequencyFromMIDINote(float noteNumber);
float samplesPerCycle(float frequency, float sample_rate);
int   bufferSizeForFrequency(float frequency, float sample_rate);

#pragma mark - Parameter Smoother

typedef struct {
    double input;
    double a;
    double b;
    double output;
} DGParameterSmoother;

void DGParameterSmoother_init(DGParameterSmoother * smoother, double rate);
void DGParameterSmoother_do(DGParameterSmoother * smoother, float * input, float * output);

//BSParameterRangePoint is used to plot the parameter response
typedef struct{
    int location;
    float value;
}BSParameterRangePoint;

typedef struct{
    
    float values[127]; //0-127 in 0-1.0f
 
}BSParameterRangeMap;


void BSParameterRangeMap_init(BSParameterRangeMap * map, BSParameterRangePoint * points, int pointsLength);
void BSParameterRangeMap_initWithJSON(BSParameterRangeMap * map, nlohmann::json mapJSON);

#pragma mark - VCA

typedef struct {
    DGParameterSmoother * smoother;
    float value;//0 - 1
} DGVCA;

void DGVCA_init(DGVCA * vca, float rate, float value);
void DGVCA_clear(DGVCA * vca);
void DGVCA_do(DGVCA * vca, float * input, float * output);

#pragma mark - Ring Buffer

typedef struct{
    
    float *  samples;
    uint64_t length;
    uint64_t index;
    
}DGRingBuffer;

void DGRingBuffer_init(DGRingBuffer * buffer, uint64_t length);
void DGRingBuffer_clear(DGRingBuffer * buffer);
void DGRingBuffer_do(DGRingBuffer * buffer, float * input, float * output);

#pragma mark - Delay

typedef struct{
    
    DGRingBuffer * buffer[2];
    float          feedback;
    float          mix;
    
    //internally used variables
    float          last_output[2];
    float          pan[2];//left and right gain values for pan
    
}DGDelay;

void DGDelay_init(DGDelay * delay, uint64_t length, float feedback, float mix);
void DGDelay_clear(DGDelay * delay);
void DGDelay_do(DGDelay * delay, float * input, float * output);

#pragma mark - Mod Delay
//Similar to delay but the delaytime can be modulated smoothly (more CPU!!!)
typedef struct{
    
    float value[2];
    float position;
    
}DGModDelayRecord;

typedef struct{
    
    float sample_rate;
    uint64_t length;
    
    DGRingBuffer * buffer[2];
    float feedback;
    float mix;
    
    float last_output[2];
    float pan[2];
    
    //setable with smooth timing
    float delayTime;
    
    //internally used variables
    float jump_size;//number of samples to jump over
    float position; //+= jumpsize, will handle the wrap over of int based writing
    //not using basic ring buffer:
    
    int last_int_position; //= (position + jump_size) - last_int_position
    DGModDelayRecord previousSample;
    
    
}DGModDelay;
void DGModDelay_init(DGModDelay * delay, float sample_rate, uint64_t length, float feedback, float mix);
void DGModDelay_clear(DGModDelay * delay);
void DGModDelay_setDelayTime(DGModDelay * delay, float delayTime);
void DGModDelay_do(DGModDelay * delay, float * input, float * output);

#pragma mark - Bit Crusher

typedef struct{
    
    int downsample;
    int bitrate;
    
    //internally used variables
    int   downsample_counter;
    float bitrate_scale;
    float last_sample[2];
    
}DGBitCrusher;

void DGBitCrusher_init(DGBitCrusher * crusher, int downsample, int bitrate);
void DGBitCrusher_setBitrate(DGBitCrusher * crusher, int bitrate);
void DGBitCrusher_do(DGBitCrusher * crusher, float * input, float * output);

#pragma mark - Basic Oscillator

#define kSinTableLength 512
typedef struct{
    
    float frequency;
    float phase;//moves from 0 - 1
    float sample_rate;
    
    float sinTable[kSinTableLength];
    
}DGBasicOscillator;

void DGBasicOscillator_init(DGBasicOscillator * oscillator, float sample_rate, float frequency);
void DGBasicOscillator_clear(DGBasicOscillator * oscillator);
void DGBasicOscillator_do(DGBasicOscillator * oscillator, float * output);

#pragma mark - Filters

typedef enum {
    
    BSFilterTypeLowPass,
    BSFilterTypeHighPass,
    
}BSFilterType;

typedef struct {
    BSFilterType type;
    double cutoff;
    double rez;
    double poles[2][4];
}BSFilter;

void BSFilter_init(BSFilter * filter, BSFilterType type, double cutoff, double resonance);
void BSFilter_do(BSFilter * filter, float * input, float * output);

#pragma mark - Resonator

typedef struct{
    
    DGDelay * delays;
    int       delay_count;
    BSFilter * output_hp_filter;
    
}DGResonator;

void DGResonator_init(DGResonator * resonator, float sample_rate, int delay_count, float * note_numbers, float * feedbacks, float * pans);
void DGResonator_clear(DGResonator * resonator);
void DGResonator_initAsTempoDelay(DGResonator * resonator, float sample_rate, int delay_count,  float tempo, int * step_numbers, float * feedbacks);
void DGResonator_clearAsTempoDelay(DGResonator * resonator);

void DGResonator_do(DGResonator * resonator, float * input, float * output);


#pragma mark - Limiter

typedef struct {
    
    DGRingBuffer * inputBuffer[2];
    
    int samples_since_last_peak[2];
    //1.0 when normal - used to duck signal
    float input_gain[2];
    float target_input_gain[2];
    float input_gain_dy[2];

    bool clipped;
    //Quarter of a sinwave 0 -> 1
    float lookup[256];
    
}DGSoftLimiter;

void DGSoftLimiter_init(DGSoftLimiter * limiter);
void DGSoftLimiter_do(DGSoftLimiter * limiter, float * input, float * output);


#endif /* BSCommonModules_hpp */
