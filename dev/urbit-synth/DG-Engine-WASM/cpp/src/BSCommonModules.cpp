//
//  BSCommonModules.mm
//  DINBlossom
//
//  Created by Karl Scholz on 6/26/17.
//  Copyright © 2017 Data Garden
//

#ifndef BSCommonModules_mm
#define BSCommonModules_mm

#import "BSCommonModules.h"

#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <string.h>

/*
typedef enum {
    BSMusicalScaleChromatic,
    BSMusicalScaleMajor,
    BSMusicalScaleNaturalMinor,
    BSMusicalScaleHarmonicMinor,
    BSMusicalScalePentatonicMajor,
    BSMusicalScalePentatonicMinor,
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
} BSMusicalScale;
*/
char* BSMusicalScale_nameForScale(BSMusicalScale scale){

    switch(scale){
        case BSMusicalScaleChromatic:{
            static char name[] = "Chromatic";
            return name;
            break;
        }
        case BSMusicalScaleMajor:{
            static char name[] = "Major";
            return name;
            break;
        }
        case BSMusicalScaleNaturalMinor:{
            static char name[] = "Natural Minor";
            return name;
            break;
        }
        case BSMusicalScaleHarmonicMinor:{
            static char name[] = "Harmonic Minor";
            return name;
            break;
        }
        case BSMusicalScalePentatonicMajor:{
            static char name[] = "Pentatonic Major";
            return name;
            break;
        }
        case BSMusicalScalePentatonicMinor:{
            static char name[] = "Pentatonic Minor";
            return name;
            break;
        }
        case BSMusicalScaleIonian:{
            static char name[] = "Ionian";
            return name;
            break;//major
        }
        case BSMusicalScaleDorian:{
            static char name[] = "Dorian";
            return name;
            break;
        }
        case BSMusicalScalePhrygian:{
            static char name[] = "Phrygian";
            return name;
            break;
        }
        case BSMusicalScaleLydian:{
            static char name[] = "Lydian";
            return name;
            break;
        }
        case BSMusicalScaleMixolydian:{
            static char name[] = "Mixolydian";
            return name;
            break;
        }
        case BSMusicalScaleAeolian:{
            static char name[] = "Aeolian";
            return name;
            break;//natural minor
        }
        case BSMusicalScaleLocrian:{
            static char name[] = "Locrian";
            return name;
            break;
        }
        case BSMusicalScaleWholeTone:{
            static char name[] = "Whole Tone";
            return name;
            break;
        }
        case BSMusicalScaleJazzMinor:{
            static char name[] = "Jazz Minor";
            return name;
            break;
        }
        case BSMusicalScaleHungarian:{
            static char name[] = "Hungarian";
            return name;
            break;
        }
        case BSMusicalScaleCount:{
            static char name[] = "Major";
            return name;
            break;
        }
    }
    static char name[] = "Major";
    return name;
}
BSMusicalScaleMap * BSMusicalScale_mapForScale(BSMusicalScale scale){

    BSMusicalScaleMap * map = new BSMusicalScaleMap;
    
    switch(scale){
        case BSMusicalScaleChromatic:{
            map->notes = new float[12]{0,1,2,3,4,5,6,7,8,9,10,11};
            map->notesLength = 12;
            break;
        }
        case BSMusicalScaleMajor:{
            map->notes = new float[7]{0,2,4,5,7,9,11};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleNaturalMinor:{
            map->notes = new float[7]{0,2,3,5,7,8,10};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleHarmonicMinor:{
            map->notes = new float[7]{0,2,3,5,7,8,11};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScalePentatonicMajor:{
            map->notes = new float[5]{0,2,4,7,9};
            map->notesLength = 5;
            break;
        }
        case BSMusicalScalePentatonicMinor:{
            map->notes = new float[5]{0,3,5,7,10};
            map->notesLength = 5;
            break;
        }
        case BSMusicalScaleIonian:{
            map->notes = new float[7]{0,2,4,5,7,9,11};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleDorian:{
            map->notes = new float[7]{0,2,3,5,7,9,10};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScalePhrygian:{
            map->notes = new float[7]{0,1,3,5,7,8,10};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleLydian:{
            map->notes = new float[7]{0,2,4,6,7,9,11};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleMixolydian:{
            map->notes = new float[7]{0,2,4,5,7,9,10};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleAeolian:{
            map->notes = new float[7]{0,2,3,5,7,8,10};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleLocrian:{
            map->notes = new float[7]{0,1,3,5,6,8,10};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleWholeTone:{
            map->notes = new float[6]{0,2,4,6,8,10};
            map->notesLength = 6;
            break;
        }
        case BSMusicalScaleJazzMinor:{
            map->notes = new float[7]{0,2,3,5,7,9,11};
            map->notesLength = 7;
            break;
        }
        case BSMusicalScaleHungarian:{
            map->notes = new float[7]{0,2,3,6,7,8,11};
            map->notesLength = 7;
            break;
        }
        default:{//chromatic by default
            std::cout << "Using default scale" << std::endl;
            map->notes = new float[12]{0,1,2,3,4,5,6,7,8,9,10,11};
            map->notesLength = 12;
            break;
        }
    }
    
    return map;
}
BSMusicalScale BSMusicalScale_scaleFromString(char * name){
    BSMusicalScale scale = BSMusicalScaleMajor;
    bool scaleSet = false;
    for(int i = 0; i < (int)BSMusicalScaleCount; i++){
        //will return A if bad match
        if(strcmp(name, BSMusicalScale_nameForScale((BSMusicalScale)i)) == 0){
            scale = (BSMusicalScale)i;
            scaleSet = true;
        }
    }
    if(!scaleSet){
        std::cout << "Couldn't find match for scale name " << name << std::endl;
        scale = BSMusicalScalePentatonicMajor;
    }
    return scale;
}


char* BSRootKey_nameForRootKey(BSRootKey key){
    
    switch(key){
        case BSRootKeyA:{
            static char name[] = "A";
            return name;
            break;
        }
        case BSRootKeyASharp:{
            static char name[] = "A#";
            return name;
            break;
        }
        case BSRootKeyB:{
            static char name[] = "B";
            return name;
            break;
        }
        case BSRootKeyC:{
            static char name[] = "C";
            return name;
            break;
        }
        case BSRootKeyCSharp:{
            static char name[] = "C#";
            return name;
            break;
        }
        case BSRootKeyD:{
            static char name[] = "D";
            return name;
            break;
        }
        case BSRootKeyDSharp:{
            static char name[] = "D#";
            return name;
            break;
        }
        case BSRootKeyE:{
            static char name[] = "E";
            return name;
            break;
        }
        case BSRootKeyF:{
            static char name[] = "F";
            return name;
            break;
        }
        case BSRootKeyFSharp:{
            static char name[] = "F#";
            return name;
            break;
        }
        case BSRootKeyG:{
            static char name[] = "G";
            return name;
            break;
        }
        case BSRootKeyGSharp:{
            static char name[] = "G#";
            return name;
            break;
        }

        case BSRootKeyCount: {
            static char name[] = "A";
            return name;
            break;
        }
    }
    static char name[] = "A";
    return name;
}

BSRootKey BSRootKey_rootKeyFromString(char * name){
    BSRootKey key = BSRootKeyA;
    bool keySet = false;
    for(int i = 0; i < 12; i++){
        if(strcmp(name, BSRootKey_nameForRootKey((BSRootKey)i)) == 0){
            key = (BSRootKey)i;
            keySet = true;
        }
    }
    
    if(keySet){
        //std::cout << "KeySet " << std::endl;

    }
    else{
        //std::cout << "Not KeySet" << std::endl;
    }
    
    //std::cout << "Key from rootKeyFromString is " << key << std::endl;
    
    return key;
}

int BSRootKey_midiNumberForRootKey(BSRootKey key){
    return 21 + (int)key;
}

void DGSample_clear(DGSample * sample){
    //delete sample->samples;
}

void BSMakePanGains(float angle, float * gains){
    
    float panAngle = angle * (0.5 * M_PI);
    float sqrt2over2 = sqrtf(2.0f)/2.0f;
    gains[0] = sqrt2over2 * (cos(panAngle) - sin(panAngle));
    gains[1] = sqrt2over2 * (cos(panAngle) + sin(panAngle));
    
}


#pragma mark - Frequency and Sample Size Math

float frequencyFromMIDINote(float noteNumber){
    return 440.0f * powf(2, (noteNumber - 69.0f) / 12.0f);
}

float samplesPerCycle(float frequency, float sample_rate){
    return (sample_rate / frequency);
}

int bufferSizeForFrequency(float frequency, float sample_rate){
    return (int)round(sample_rate / frequency);
}

#pragma mark - Parameter Smoother

void DGParameterSmoother_init(DGParameterSmoother * smoother, double rate){
    
    if(rate >= 1.0)
        rate = 0.999999999;

    if(rate < 0.0)
        rate = 0.0;
    
    smoother->input = 0;
    smoother->a = rate;
    smoother->b = 1.0f - rate;
    smoother->output = 0.0;
    
}

void DGParameterSmoother_do(DGParameterSmoother * smoother, float * input, float * output){
    smoother->input = (double)*input;
    smoother->output = (smoother->input * smoother->b) + (smoother->output * smoother->a);
    *output = (float)smoother->output;
}

#pragma mark - BSParameterRangeMap

// Comaprision function. Returns <0 for a<b =0 for a=b and >0 for a>b
static int BSParameterRangePoint_compare( const void *a, const void *b )
{
    
    // Cast to the correct pointer type
    const BSParameterRangePoint *p1 = (const BSParameterRangePoint*) a;
    const BSParameterRangePoint *p2 = (const BSParameterRangePoint*) b;
    
    // There are different ways of comparing floats to each other. In this case we use |a - b| < epsilon for some small epsilon
    int difference = p2->location - p1->location;
    
    //puts lowest values first
    if( difference <= 0)
    {
        return 1;
    }
    else if( difference >= 0)
    {
        return -1;
    }
    
    return 0;
    
}

void BSParameterRangeMap_initWithJSON(BSParameterRangeMap * map, nlohmann::json mapJSON){
    
    //[[0,0.0],[127,1.0]]
    int numPoints = (int)mapJSON.size();
    
    BSParameterRangePoint points[numPoints];
    
    for(int i = 0; i < numPoints; i++){
        
        points[i].location = (int)mapJSON[i][0];
        points[i].value = (float)mapJSON[i][1];
                
    }
    
    BSParameterRangeMap_init(map, points, numPoints);
    
}

void BSParameterRangeMap_init(BSParameterRangeMap * map, BSParameterRangePoint * points, int pointsLength){
    
    qsort(points, pointsLength, sizeof(BSParameterRangePoint), &BSParameterRangePoint_compare);
    
    //populate map array of floats
    for(int i = 0; i < pointsLength; i++){
        
        //first element in points - backfill all location to its value
        if(i == 0){
            for(int j = 0; j < points[i].location; j++){
                map->values[j] = points[i].value;
            }
        }
        
        //linear interpolation of points into values array
        if(i < pointsLength - 1){
         
            int distance = points[i+1].location - points[i].location;
            for(int j = 0; j < distance; j++){
                float deltaPerStep = (points[i+1].value - points[i].value) / (float)distance;
                map->values[points[i].location + j] = points[i].value + (deltaPerStep * (float)j);
            }
            
        }
        
        //fill end of array to end
        if(i == pointsLength -1){
            for(int j = points[i].location; j < 128; j++){
                map->values[j] = points[i].value;
            }
        }
        
    }

}

void testRangeMap(){
 
    BSParameterRangePoint points[3];
    
    points[0].location = 64;
    points[0].value = 1.0;
    
    points[1].location = 10;
    points[1].value = 0.5;
    
    points[2].location = 120;
    points[2].value = 0.2;
    
    BSParameterRangeMap map;
    
    BSParameterRangeMap_init(&map, points, 3);
    
    std::cout << "Test range map" << std::endl;
    
}


#pragma mark - VCA

void DGVCA_init(DGVCA * vca, float rate, float value){
    
    //testRangeMap();
    
    vca->smoother = new DGParameterSmoother;
    DGParameterSmoother_init(vca->smoother, rate);
    vca->value = value;
    
}

void DGVCA_clear(DGVCA * vca){
    
    delete vca->smoother;
    
}

void DGVCA_do(DGVCA * vca, float * input, float * output){

    float vca_output = 0.0;
    DGParameterSmoother_do(vca->smoother, &vca->value, &vca_output);
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = input[chan] * vca_output;
    }
    
}

#pragma mark - Ring Buffer

void DGRingBuffer_init(DGRingBuffer * buffer, uint64_t length){
    
    //store the length for use later
    buffer->length = length;
    
    //allocate the float array for holding the samples
    buffer->samples = new float[buffer->length];
    for(int i = 0; i < buffer->length; i++){
        buffer->samples[i] = 0.0f;
    }
    
    //start the index at 0
    buffer->index = 0;
    
}

void DGRingBuffer_clear(DGRingBuffer * buffer){
    delete buffer->samples;
}

void DGRingBuffer_do(DGRingBuffer * buffer, float * input, float * output){
    
    //read the old sample from the index address
    *output = buffer->samples[buffer->index];
    
    //write to that index with the new sample
    buffer->samples[buffer->index] = *input;
    
    //update the index
    buffer->index++;
    
    //if the index is has passed the length of the array, wrap back to 0
    if(buffer->index >= buffer->length){
        buffer->index = 0;
    }
    
}

#pragma mark - Delay

void DGDelay_init(DGDelay * delay, uint64_t length, float feedback, float mix){
    
    delay->feedback = feedback;
    
    delay->mix = mix;
    
    for(int chan = 0; chan < 2; chan++){
        delay->buffer[chan] = new DGRingBuffer;
        DGRingBuffer_init(delay->buffer[chan], length);
        delay->last_output[chan] = 0.0f;
    }
    
    BSMakePanGains(0.0, delay->pan);
    
}

void DGDelay_clear(DGDelay * delay){
    for(int chan = 0; chan < 2; chan++){
        DGRingBuffer_clear(delay->buffer[chan]);
        delete delay->buffer[chan];
    }
}

void DGDelay_do(DGDelay * delay, float * input, float * output){
    
    float signal_going_to_delay[2] = {0.0f, 0.0f};
    
    for(int chan = 0; chan < 2; chan++){
        signal_going_to_delay[chan] = input[chan] + (delay->last_output[chan] * delay->feedback);
        DGRingBuffer_do(delay->buffer[chan], &signal_going_to_delay[chan], &delay->last_output[chan]);
        output[chan] = (delay->last_output[chan] * delay->pan[chan] * delay->mix) + (input[chan] * (1.0f - delay->mix));
    }
    
}

#pragma mark - Mod Delay

void DGModDelay_init(DGModDelay * delay, float sample_rate, uint64_t length, float feedback, float mix){
    
    delay->feedback = feedback;
    
    delay->mix = mix;
    
    for(int chan = 0; chan < 2; chan++){
        delay->buffer[chan] = new DGRingBuffer;
        DGRingBuffer_init(delay->buffer[chan], length);
        delay->last_output[chan] = 0.0f;
    }
    
    BSMakePanGains(0.0, delay->pan);
    
    delay->length = length;
    delay->sample_rate = sample_rate;
    delay->position = 0.0;
    
    delay->previousSample.value[0] = 0.0;
    delay->previousSample.value[1] = 0.0;
    delay->previousSample.position = 0.0;

}

void DGModDelay_clear(DGModDelay * delay){
    
    for(int chan = 0; chan < 2; chan++){
        DGRingBuffer_clear(delay->buffer[chan]);
        delete delay->buffer[chan];
    }
    
}

void DGModDelay_setDelayTime(DGModDelay * delay, float delayTime){
    
    float bufferLengthInSeconds = (float)delay->sample_rate / delay->sample_rate;
    delay->jump_size = bufferLengthInSeconds / delayTime;
    if(delay->jump_size < 2){
        delay->jump_size = 2;
    }
    
}

void DGModDelay_do(DGModDelay * delay, float * input, float * output){
    
    
    //tracking position in delay
    delay->position += delay->jump_size;
    
    int intJumpSize = (int)delay->position - (int)delay->previousSample.position;
    
    float startingDx = (float)((int)delay->previousSample.position + 1) - delay->previousSample.position;
    
    for(int chan = 0; chan < 2; chan++){
        
        float mixedInput = input[chan] + (delay->last_output[chan] * delay->feedback);

        //determine sample dy per jump step;
        //store previous sample because we might mess with it
        float previous_sample = delay->previousSample.value[chan];
        float dy = (mixedInput - previous_sample) / delay->jump_size;
        
        float startingDy = startingDx * dy;
        
        float interSamples[2] = {0.0, 0.0};
        for(int i = 0; i < intJumpSize; i++){
        
            float to_delay = previous_sample + startingDy + (dy * i);
            
            float from_delay = 0.0f;
            DGRingBuffer_do(delay->buffer[chan], &to_delay, &from_delay);
            
            //save first two samples read for interpolating output
            if(i < 2){
                interSamples[i] = from_delay;
            }

        }
        
        //output = interpolated value at last location
        //starting dx is the location between 0 and 1 of the last
        //position
        if(intJumpSize > 1){
            delay->last_output[chan] = (interSamples[1] * startingDx) + (interSamples[0] * (1 - startingDx));
        }
        else{
            delay->last_output[chan] = interSamples[0];
        }
        
        delay->previousSample.value[chan] = mixedInput;

        
        //
        
        if(chan == 0){
            output[chan] = input[chan];
        }
        else{
            output[chan] = (delay->last_output[chan] * delay->pan[chan] * delay->mix) + (input[chan] * (1.0f - delay->mix));
        }

    }
    
    //store last sample
    delay->previousSample.position = delay->position;
    
    //just to prevent the float based value from running out of control and to keep positions positive
    if(delay->position > 10000.0f){
        delay->position -= 9000.0f;
        delay->previousSample.position -= 9000.0f;
    }

}

#pragma mark - Bit Crusher

void DGBitCrusher_init(DGBitCrusher * crusher, int downsample, int bitrate){
    
    crusher->downsample = downsample;

    DGBitCrusher_setBitrate(crusher, bitrate);
    
    crusher->downsample_counter = 0;
    for(int chan = 0; chan < 2; chan++){
        crusher->last_sample[chan] = 0.0f;
    }
    
}

void DGBitCrusher_setBitrate(DGBitCrusher * crusher, int bitrate){
    
    crusher->bitrate = bitrate;
    crusher->bitrate_scale = powf(2, (bitrate - 1));
    
}

void DGBitCrusher_do(DGBitCrusher * crusher, float * input, float * output){
    
    //REDUCING SAMPLE RATE
    //To reduce the sample rate, we count samples and keep writing the same data to the output until we pass the 'downsample' amount
    //downsample values of 0 or 1 will pass all samples through
    
    float signal[2] = {input[0], input[1]};
    
    for(int chan = 0; chan < 2; chan++){
        if(signal[chan] > 1.0){
            signal[chan] = 1.0;
        }
        
        if(signal[chan] < -1.0){
            signal[chan] = -1.0;
        }
    }
    
    //we increment the downsample_counter on every 'do'
    crusher->downsample_counter++;
    
    if(crusher->downsample_counter >= crusher->downsample){
        for(int chan = 0; chan < 2; chan++){
            //REDUCING BIT DEPTH
            //to lower the bitrate we scale the (-1 ... 1) float by the bitrate_scale, quantize the value to integers, and scale it back down
            int quantized_value = (int)(signal[chan] * crusher->bitrate_scale);
            float bit_reduced_sample = ((float)quantized_value) / crusher->bitrate_scale;
            
            crusher->last_sample[chan] = bit_reduced_sample;
            crusher->downsample_counter = 0;
        }
        
    }
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = crusher->last_sample[chan];
    }
    
}

#pragma mark - Basic Oscillator

void DGBasicOscillator_init(DGBasicOscillator * oscillator, float sample_rate, float frequency){
    
    oscillator->sample_rate = sample_rate;
    oscillator->phase = 0;
    oscillator->frequency = frequency;
    
    for(int i = 0; i < kSinTableLength; i++){
        oscillator->sinTable[i] = sinf(2.0f * M_PI * (float)i / (float) kSinTableLength);
        //std::cout << "Writing sin table " << oscillator->sinTable[i] << std::endl;
    }
    
}

void DGBasicOscillator_do(DGBasicOscillator * oscillator, float * output){
    
    //look up from table
    unsigned int tableIndex = (int)((float)(kSinTableLength - 1) * oscillator->phase);
    
    if(tableIndex < 0){
        tableIndex = 0;
    }

    float out_sample = oscillator->sinTable[tableIndex];
    
    //sawtooth -1 to 1
    for(int chan = 0; chan < 2; chan++){
        output[chan] = out_sample;
    }
    
    float spc = samplesPerCycle(oscillator->frequency, oscillator->sample_rate);
    
    oscillator->phase += 1.0f / spc;
    
    while(oscillator->phase >= 1.0)
        oscillator->phase -= 1.0;
    
    if(oscillator->phase <= 0.0)
        oscillator->phase = 0.0;
    
}

#pragma mark - Resonator

void DGResonator_init(DGResonator * resonator, float sample_rate, int delay_count, float * note_numbers, float * feedbacks, float * pans){
    
    resonator->delay_count = delay_count;
    resonator->delays = new DGDelay[delay_count];
    
    resonator->output_hp_filter = new BSFilter;
    BSFilter_init(resonator->output_hp_filter, BSFilterTypeHighPass, 0.3, 0.0);
    
    for(int i = 0; i < delay_count; i++){
        
        //determine buffer size of delay
        int buffer_size = bufferSizeForFrequency(frequencyFromMIDINote(note_numbers[i]), sample_rate);
        
        DGDelay_init(&resonator->delays[i], buffer_size, feedbacks[i], 0.8);
        
        BSMakePanGains(pans[i], resonator->delays[i].pan);
        
    }
}

void DGResonator_clear(DGResonator *resonator){
    
    for(int i = 0; i < resonator->delay_count; i++){
        
        DGDelay_clear(&resonator->delays[i]);
        
    }
    
    delete resonator->delays;
}




void DGResonator_initAsTempoDelay(DGResonator * resonator, float sample_rate, int delay_count,  float tempo, int * step_numbers, float * feedbacks){
    
    resonator->delay_count = delay_count;
    resonator->delays = new DGDelay[delay_count];
    
    resonator->output_hp_filter = new BSFilter;
    BSFilter_init(resonator->output_hp_filter, BSFilterTypeHighPass, 0.3, 0.0);
    
    for(int i = 0; i < delay_count; i++){
        
        //determine buffer size of delay
        float seconds_per_step = (60.0f / (tempo * 4.0f));
        float seconds_for_delay = seconds_per_step * step_numbers[i];
        float freq = 1.0f / seconds_for_delay;

        int buffer_size = bufferSizeForFrequency(freq, sample_rate);
        
        DGDelay_init(&resonator->delays[i], buffer_size, feedbacks[i], 0.8);
        
    }
    
}

void DGResonator_clearAsTempoDelay(DGResonator * resonator){
    
    DGResonator_clear(resonator);
    
    delete resonator->output_hp_filter;
}


void DGResonator_do(DGResonator * resonator, float * input, float * output){
    
    float output_sample[2] = {0.0f, 0.0f};
    
    for(int i = 0; i < resonator->delay_count; i++){
        
        float individual_output[2] = {0.0f, 0.0f};
        
        DGDelay_do(&resonator->delays[i], input, individual_output);
        
        for(int chan = 0; chan < 2; chan++){
            output_sample[chan] += individual_output[chan];
        }
        //output_sample = *input;
    }
    
    BSFilter_do(resonator->output_hp_filter, output_sample, output_sample);
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = output_sample[chan];
    }
    
}

#pragma mark - Filters

void BSFilter_init(BSFilter * filter, BSFilterType type, double cutoff, double resonance){
    for(int chan = 0; chan < 2; chan++){
        for(int i = 0; i < 4; i++){
            filter->poles[chan][i] = 0;
        }
    }
    filter->rez = resonance;
    filter->cutoff = cutoff;
    filter->type = type;
}

void BSFilter_do(BSFilter * filter, float * input, float * output){
    
    double pad = 24.0f;
    
    double cutoff = filter->cutoff;
    
    if(cutoff > 0.99) {cutoff = 0.99;}
    if(cutoff < 0.001) {cutoff = 0.001;}

    //NSLog(@"filter input %f, cutoff %f, rez %f", input, cutoff, filter->rez);
    for(int chan = 0; chan < 2; chan++){
        
        double incoming = (double)input[chan] / pad;
        
        double rez_t = filter->poles[chan][3] * filter->rez;
        
        if (rez_t > 1) {rez_t = 1;}
        
        incoming = incoming - rez_t;
        
        filter->poles[chan][0] = filter->poles[chan][0] + (((-1 * filter->poles[chan][0]) + incoming) * cutoff);
        filter->poles[chan][1] = filter->poles[chan][1] + (((-1 * filter->poles[chan][1]) + filter->poles[chan][0]) * cutoff);
        filter->poles[chan][2] = filter->poles[chan][2] + (((-1 * filter->poles[chan][2]) + filter->poles[chan][1]) * cutoff);
        filter->poles[chan][3] = filter->poles[chan][3] + (((-1 * filter->poles[chan][3]) + filter->poles[chan][2]) * cutoff);
        
        if(filter->type == BSFilterTypeLowPass){
            output[chan] = (float)((filter->poles[chan][3] * (filter->rez + 1)) * pad);
        }
        else if(filter->type == BSFilterTypeHighPass){
            output[chan] = (float)((incoming - (filter->poles[chan][3] * (filter->rez + 1))) * pad);
        }
    }
}

#pragma mark - Limiter

void DGSoftLimiter_init(DGSoftLimiter * limiter){
    


    for(int chan = 0; chan < 2; chan++){
        
        limiter->samples_since_last_peak[chan] = 0;
        
        limiter->input_gain[chan] = 1.0;
        limiter->target_input_gain[chan] = 1.0;
        limiter->input_gain_dy[chan] = 0.0;
        
        limiter->inputBuffer[chan] = new DGRingBuffer;
        DGRingBuffer_init(limiter->inputBuffer[chan], 1024);
    }
    
    
    limiter->clipped = false;
    
    //generate a quarter of  sine wave 0 - 1 for looking up.
    for(int i = 0; i < 256; i++){
        
        float angle = (((float)i / 255.0f) * 0.5) * M_PI;
        
        limiter->lookup[i] = sinf(angle);
        
    }
    
    
}
void DGSoftLimiter_do(DGSoftLimiter * limiter, float * input, float * output){
    
    float maxLevel = 0.999;
    float headRoom = (1.0f - maxLevel);
    
    for(int chan = 0; chan < 2; chan++){
        
        float absolute_prebuffer = fabs(input[chan]);
    
        
        if(absolute_prebuffer > maxLevel){
            
            if(limiter->samples_since_last_peak[chan] > 1024){
                limiter->target_input_gain[chan] = 1.0;
            }
            
            float potential_target_input_gain = maxLevel / absolute_prebuffer;
            
            if(potential_target_input_gain < limiter->target_input_gain[chan]){
                limiter->target_input_gain[chan] = potential_target_input_gain;
                limiter->samples_since_last_peak[chan] = 0;
            }
            

            
            float potential_input_gain_dy = (limiter->target_input_gain[chan] - limiter->input_gain[chan]) / 1024.0f;
            
            if(potential_input_gain_dy < limiter->input_gain_dy[chan]){
                limiter->input_gain_dy[chan] = potential_input_gain_dy;
            }
            //we have 1024 samples to lower the volume
        }
        //we've reached the desired input attenuation - start to ramp back up
        if(limiter->input_gain_dy[chan] < 0.0 && limiter->input_gain[chan] < limiter->target_input_gain[chan]){
            
            //setting to 1.0 is wrong because we may have a new target input gain from a loud event.
            limiter->target_input_gain[chan] = 1.0;
            limiter->input_gain_dy[chan] = (limiter->target_input_gain[chan] - limiter->input_gain[chan]) / (1024 * 12.0f);
        }
        
        //updating input gain
        limiter->input_gain[chan] += limiter->input_gain_dy[chan];
        
        if(limiter->input_gain[chan] > 1.0){
            limiter->input_gain[chan] = 1.0;
        }
        
        limiter->samples_since_last_peak[chan]++;

        //write to ring buffer - note if input is over max level
        float from_buffer = 0.0;
        DGRingBuffer_do(limiter->inputBuffer[chan], &input[chan], &from_buffer);

        from_buffer = from_buffer * limiter->input_gain[chan];
        //if signal is over max level
        
        float absolute_input = fabs(from_buffer);
        
        if(absolute_input > maxLevel){
            
            limiter->clipped = true;
            
            float amount_over = absolute_input - maxLevel;
            
            float scaled_amount_over = (255.0f / headRoom) * amount_over;
            
            int lookup_index = (int)scaled_amount_over;
            
            if(lookup_index > 255){
                lookup_index = 255;
            }
            
            float absolute_output = maxLevel + (limiter->lookup[lookup_index] * headRoom);
            
            //restore polarity
            float polarity = from_buffer / absolute_input;
            
            output[chan] = absolute_output * polarity;
            
        }
        else{
            output[chan] = from_buffer;
        }
    }
}

#endif /* BSCommonModules_mm */
