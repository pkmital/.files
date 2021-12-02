//
//  DGWavetableSynth.m
//  DINBlossom
//
//  Created by Karl Scholz on 7/7/17.
//  Copyright © 2017 Data Garden
//

#ifndef DGWavetableSynth_cpp
#define DGWavetableSynth_cpp

#import "DGWavetableSynth.h"
#ifdef IOS
#import "DGEngineManager-C-Interface.h"
#endif
//#import "AudioFileLoader.h"

#include <math.h>       /* cos */
#include <string>
#include <sstream>
#include <algorithm>
#include <tgmath.h>
#import "ResamplerFlt.h"
#include <bsd/stdlib.h>

using std::string;

inline float interpolate_samples(float sample1, float sample2, float position){
    return (sample1 * (1.0f - position)) + (sample2 * position);
}

void DGSampleVoice_init(DGSampleVoice * voice){
    
    //std::cout << "Sample voice init" << std::endl;

    voice->tuningFrequency = 440.0f;
    voice->playing = false;
    voice->gain = 1.0f;
    
}

void DGSampleVoice_updateTuningFrequency(DGSampleVoice * voice){
    
    float local_transpose = voice->note - voice->rootNote;
    
    double baseFrequencyRatio = voice->tuningFrequency / 440.0f;

    //semitone frequency ratio = 2 ^ (1 / 12)
    
    //read_index_increment partially deprecated after incorporating rspl library
    voice->read_index_increment = powf(2, local_transpose / 12.0f);
    voice->read_index_increment *= baseFrequencyRatio;
    
    //voice->pitch_rspl = local_transpose * 0x1555;
    
    //solving for baseFrequencyRatio to semitones
    double semitone_shift_for_tuning_frequency = 12.0f * log2(baseFrequencyRatio);
    local_transpose += semitone_shift_for_tuning_frequency;
    //we're dealing withints and could lose precision to rounding so multiply semitones by the octave hex (0x10000L) and divide by 12 before rounding
    voice->pitch_rspl = (long)(((double)local_transpose * (double)0x10000L) / (double)12.0f);
}

rspl::InterpPack interp_pack_local;

void DGSampleVoice_trigger(DGSampleVoice * voice, DGSample * sample, int note){
    
    //std::cout << "Sample voice trigger: " << note << std::endl;

    voice->sample = sample;
    voice->read_index = 0.0f;
    voice->read_index_literal = 0;
    
    voice->resampler_flt.set_sample (sample->mip_map);
    voice->resampler_flt.set_interp (interp_pack_local);
    voice->resampler_flt.clear_buffers ();

    
    while(note > 127){
        note -= 36;
    }
    while(note < 12){
        note += 12;
    }
    
    //mapping volume
    //approximating fletch munson curve so our notes are not too annoying
    //https://en.wikipedia.org/wiki/Fletcher%E2%80%93Munson_curves
    
    //Note numbers 68/69 are the most sensitive
    float fletcher_munsom_psuedo_scale = 0.0f;
    if(note < 69){
        fletcher_munsom_psuedo_scale = powf(1.0f - ((float)note / 69.0f), 2.0f);
    }
    else{
        float note_scaled = (float)(note - 69) / (58.0f);
        fletcher_munsom_psuedo_scale = (powf(note_scaled, 2.0f) * 0.1);
    }
    
    float down_ramp_scale = powf(1.0f - ((float)note / 127.0f), 3.0f);
    voice->gain = (fletcher_munsom_psuedo_scale + down_ramp_scale) / 2.0f;
    
    //determine the read_index_increment from note
    voice->rootNote = voice->sample->rootNote;
    voice->note = (float)note;
    
    DGSampleVoice_updateTuningFrequency(voice);
    
    //std::cout << "Sample voice trigger voice->read_index_increment: " << voice->read_index_increment << std::endl;
    float random_range = ((float)(arc4random() % 256) / 128.0f) - 1.0;
    float panAngle = random_range * 0.1 * M_PI;
    BSMakePanGains(panAngle, voice->pan);
    
    voice->playing = true;

}

//reading from DGSample
void DGSampleVoice_do(DGSampleVoice * voice, float * output){
    DGSample * sample = voice->sample;

    //catch
    if(voice->read_index >= ((float)sample->length - 1.0f)){
        voice->playing = false;
        return;
    }
    
    uint64_t int_index = (uint64_t)voice->read_index;
    float sample1 = sample->samples[int_index];
    float sample2 = sample->samples[int_index + 1];
    float position = voice->read_index - (float)int_index;
    
    //old interpolation
    /*
    *output = (voice->gain * interpolate_samples(sample1, sample2, position));
    voice->read_index += voice->read_index_increment;
    if(voice->read_index > sample->length - 16){
        voice->playing = false;
    }
    return;
     */
    //end of old interpolation
    
    
    voice->read_index += voice->read_index_increment;
    
    //
    //std::vector <float> out_sig (1, 0.0);
        
    float out_sig[1] = {0.0};
    out_sig[0] = 0.0;
    
    
    const long renderedLength = (long)((float)sample->length / voice->read_index_increment);
    
    const long        pitch = 0;
    voice->resampler_flt.set_pitch(voice->pitch_rspl);
    
    if((voice->resampler_flt.get_playback_pos() >> 32) < sample->length - 16){
        voice->resampler_flt.interpolate_block (&out_sig [0], 1);
    }
    else{
        voice->playing = false;
        return;
    }
    *output = out_sig[0];
    
    voice->read_index_literal++;
}

int indexOfRegionForNote(DGWavetableSynth * synth, int note){
    
    for(int i = 0; i < synth->region_count; i++){
        DGSampleRegion * region = &synth->regions[i];
        if(note >= region->lowNote && note <= region->highNote){
            return i;
        }
    }
    
    //if we didn't find a region to match - just use the first one
    return 0;
}

void DGWavetableSynth_playNote(DGWavetableSynth * synth, int note){
    
    //std::cout << "DGWavetableSynth_playNote: " << note << std::endl;
#ifdef IOS
    arpeggiatorBusEvent(note, synth->synthId);
#endif
    //determine which region coresponds to note
    DGSampleRegion * region = &synth->regions[indexOfRegionForNote(synth, note)];
    
    //default is to use the cycling voices
    int voiceToUse = synth->voice_index_to_trigger;
    
    //first check to see if any voices are free so we don't have to interrupt
    for(int i = 0; i < synth->polyphony; i++){
        if(!synth->voices[i].playing){
            
            //std::cout << "DGWavetableSynth_playNote: voice to use " << i << std::endl;

            voiceToUse = i;
            break;
        }
    }
    
    if(!synth->voices[voiceToUse].playing){
        DGSampleVoice_trigger(&synth->voices[voiceToUse], region->sample, note);
        
        synth->voice_index_to_trigger++;
        if(synth->voice_index_to_trigger >= synth->polyphony){
            synth->voice_index_to_trigger = 0;
        }
    }
    
}

void DGWavetableSynth_init(DGWavetableSynth * synth){
    
    synth->polyphony = 8;
    synth->voice_index_to_trigger = 0;
    
    synth->loaded = false;
    synth->regionsSet = false;
    synth->voices = new DGSampleVoice[synth->polyphony];
    
    for(int i = 0; i < synth->polyphony; i++){
        DGSampleVoice_init(&synth->voices[i]);
    }
    
}

void DGWavetableSynth_clear(DGWavetableSynth * synth){


    for(int i = 0; i < synth->polyphony; i++){
        DGSampleVoice_clear(&synth->voices[i]);
    }
    
    //delete synth->voices;
    
    

    for(int i = 0; i < synth->region_count; i++){
        DGSampleRegion_clear(&synth->regions[i]);
    }
    
    delete synth->regions;
    
}

void DGSampleVoice_clear(DGSampleVoice * voice){
    voice->sample = NULL;
}

void DGSampleRegion_clear(DGSampleRegion * region){

    DGSample_clear(region->sample);
    delete region->sample;
    
}


void DGWavetableSynth_setTuningFrequency(DGWavetableSynth * synth, float frequency){
    
    for(int i = 0; i < synth->polyphony; i++){
        synth->voices[i].tuningFrequency = frequency;
        DGSampleVoice_updateTuningFrequency(&synth->voices[i]);
    }
    
}

void DGWavetableSynth_do(DGWavetableSynth * synth, float * output){
    
    float totalOutput[2] = {0.0f,0.0f};

    if(synth->loaded){
        for(int i = 0; i < synth->polyphony; i++){
            
            if(synth->voices[i].playing){
                float localOutput = 0.0f;
                DGSampleVoice_do(&synth->voices[i], &localOutput);
                for(int chan = 0; chan < 2; chan++){
                    totalOutput[chan] += (localOutput * synth->voices[i].pan[chan]);
                }
            }
            
        }
    }
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = totalOutput[chan];
    }

}

DGWavetableSynthManager::DGWavetableSynthManager(std::map<std::string,std::vector<std::string>>paths){
    //init code
    samplePaths = paths;
}

DGWavetableSynthManager::~DGWavetableSynthManager(){
    //delete code
}

//map of note ranges generated by list of multi-sample file names for a given sample
//
typedef struct{
    int lowest_note;
    int highest_note;
    int root_note;
}DGLocalNoteRangeStruct;

int rootNoteFromFilePath(std::string filepath){
    
    //file path looks like "/path/to/audio/guitar 1_60.wav"
    
    //remove file extension to make "/path/to/audio/guitar 1_60"
    std::string noExt = filepath.substr(0, filepath.find_last_of("."));
    
    //remove path to make "guitar 1_60"
    std::string noPath = noExt.substr(noExt.find_last_of("/") + 1);
    
    //std::cout << "noPath " << noPath << std::endl;

    //if we have a root note declaration in the file name (an underscore)
    if (noPath.find("_") != std::string::npos) {
        //remove filename and underscore to get "60"
        std::string rootNumberString = noPath.substr(noPath.find_last_of("_") + 1);
        
        //convert the string to an integer
        std::stringstream converter(rootNumberString);
        
        int rootNote = 0;
        converter >> rootNote;
        
        return rootNote;
    }
    //otherwise return default root
    else{
        return 60;
    }
}

std::map<std::string,DGLocalNoteRangeStruct>noteRangeMapFromFilePaths(std::vector<std::string> filePaths){
    
    std::map<std::string,DGLocalNoteRangeStruct> map;
    
    for(int i = 0; i < filePaths.size(); i++){
        
        //defaults - modified in the next loop
        map[filePaths[i]].lowest_note = 0;
        map[filePaths[i]].highest_note = 127;
        
        //set root note from string
        map[filePaths[i]].root_note = rootNoteFromFilePath(filePaths[i]);
    }
    
    for(int i = 0; i < filePaths.size(); i++){
        //if we are addressing a multisample that is not the top one
        if(i < filePaths.size() - 1){
            //set highest note halfway between this root and next root
            int thisRoot = map[filePaths[i]].root_note;
            int nextRoot = map[filePaths[i + 1]].root_note;
            int highestNote = (thisRoot + nextRoot) / 2;
            
            map[filePaths[i]].highest_note = highestNote;
            map[filePaths[i + 1]].lowest_note = highestNote+1;
                        
        }
    }
    
    return map;
    
}

void DGWavetableSynthManager::loadSampleIntoSynth(DGWavetableSynth * synth, nlohmann::json audiofilejson){
    
    if(!audiofilejson["type"].is_null()){
        string embeddedString = "embedded";
        string typeString = audiofilejson["type"];
        if((embeddedString.compare(typeString) == 0)){
            
            std::string name = audiofilejson["name"];

            //filepaths for this sample
            std::vector<std::string> filePaths = samplePaths[name];
            
            int regionCount = (int)filePaths.size();
                
            if(!synth->regionsSet){ //#check on ipad
                synth->region_count = regionCount;
                synth->regions = new DGSampleRegion[regionCount];
                synth->regionsSet = true;
            }
            
            std::map<std::string,DGLocalNoteRangeStruct> noteRanges =
            noteRangeMapFromFilePaths(filePaths);
            
            for(int i = 0; i < regionCount; i++){
                //
                DGSampleRegion * region = &synth->regions[i];
                
                region->sample = new DGSample;
                
                region->lowNote = noteRanges[filePaths[i]].lowest_note;
                region->highNote = noteRanges[filePaths[i]].highest_note;
                region->sample->rootNote = noteRanges[filePaths[i]].root_note;
                                                
                //Create AudioFile for loading into sample array
                AudioFile<double> * audioFile = new AudioFile<double>;
                
                std::string name = audiofilejson["name"];
                std::string path = samplePaths[name][i];
                //copy samples
                if(!audioFile->load(path.c_str())){
                    std::cout << "Error loading audio file at: " << path << std::endl;
                }
                else{
                    //std::cout << "Success loading audio file at: " << path << std::endl;
                }
                
                int numChannels = 1;
                int sampleLength = audioFile->getNumSamplesPerChannel();
                region->sample->length = sampleLength;
                //region->sample->samples = new float[numChannels * sampleLength];

                for(int q = 0; q < sampleLength; q++){
                    region->sample->samples.push_back((float)audioFile->samples[0][q]);
                    //region->sample->samples[q] = (float)audioFile->samples[0][q];
                }
                
                region->sample->mip_map.init_sample(sampleLength, rspl::InterpPack::get_len_pre(), rspl::InterpPack::get_len_post(), 6, rspl::ResamplerFlt::_fir_mip_map_coef_arr, rspl::ResamplerFlt::MIP_MAP_FIR_LEN);
                
                region->sample->mip_map.fill_sample (&region->sample->samples [0], sampleLength);

                
                //dispose of AudioFile
                delete (audioFile);

//
//                //read embedded data
//                if(loadSampleDataFromJSONFile){
//                    region->sample->length = audiofilejson["length"];
//                    region->sample->samples = new float[channels * region->sample->length];
//                    for(int s = 0; s < region->sample->length; s++){
//                        for(int chan = 0; chan < channels; chan++){
//                            int intsample = audiofilejson["data"][s];
//                            region->sample->samples[s] = (float)intsample / (float)INT16_MAX;
//                        }
//                    }
//                }
                 
            }
        }
    }

    

    
    synth->loaded = true;
    
}

DGWavetableSynth * DGWavetableSynthManager::newSynthWithSamples(nlohmann::json audiofilejson){
    
    DGWavetableSynth * synth = new DGWavetableSynth;
    DGWavetableSynth_init(synth);
    

    DGWavetableSynthManager::loadSampleIntoSynth(synth, audiofilejson);
    
    return synth;
    
}

#endif
