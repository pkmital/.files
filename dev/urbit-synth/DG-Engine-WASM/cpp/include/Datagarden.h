//
//  Datagarden.h
//  DatagardenSandbox_iOS
//
//  Created by Karl Scholz on 10/13/18.
//  Copyright © 2018 Data Garden. All rights reserved.
//

#ifndef Datagarden_h
#define Datagarden_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#import "DGVoices.h"

#import "AudioFile.h"
#import "BSCommonModules.h"
#import "DGWavetableSynth.h"
#import "DGArpeggiator.h"

#import "json.hpp"

#import "DGTuningParameters.h"

typedef struct{
    
    float lastIBIValue;//seems to be set to zero so not touching incase it messes with note generation
    float IBIMostRecentValue;
    DGParameterSmoother * IBIRollingAverageSmoother;
    int last10notes[10];
    int last10noteIndex;
    int noteThinning;
    int noteShift;
    int lastSection;
    
}DGAudioEngineInputProcessor;

typedef struct{
    
    BSMidiMessage * messages;
    int messagesLength;

}DGAudioEngineMIDIScheduler;

BSMidiMessage midiMessageForCC(int ccnumber, int value);
BSMidiMessage midiMessageForNoteOn(int note);
BSMidiMessage midiMessageForNoteOff(int note);

class DGAudioEngine{
    
public:
    
    DGAudioEngine();
    ~DGAudioEngine();

    void initialize(nlohmann::json json_state, std::map<std::string, std::vector<std::string>> samplePaths);
    void clearBuffers();

    nlohmann::json readJSONintoState(std::string filepath);
    void switchToDMT(std::string folderpath);
    void updateRealtimeParameters(nlohmann::json jsonData);
    void updateRealtimeParametersReverb(nlohmann::json jsonData);
    void reloadVoice(int voiceNumber, nlohmann::json jsonData);
    char* currentDMTName();
    char* currentRootKey();
    char* currentScale();
    float currentRootFrequency();
    float currentTempo();
    void incrementRootKey(int direction);
    void incrementScale(int direction);
    
    void start();
    void pause();
    
    void startPlayback();
    void stopPlayback();
    bool playbackEnabled();
    
    void mute(bool mute);
    bool muted();

    void renderAudio(float *audioBuffer, uint32_t numFrames, uint32_t numChannels, double currentTime);
    
    void midi(BSMidiMessage * message);
    void midiFlush();

    void scheduleMIDIMessage(BSMidiMessage message);

    void setTuningFrequency(float frequency);
    
    void enableMultimode(bool enable);
    void setMultimodeIdForVoice(int multimode_id, int voice);
    int  getMultimodeIdForVoice(int voice);


private:
    bool running;
    
    //voice modules will live here - for testing purposes we are declaring the contents of a voice here:
    DGAudioGraph * graph;
    DGAudioEngineInputProcessor * inputProcessor;
    DGAudioEngineMIDIScheduler * midiScheduler;
};

#endif /* Datagarden_h */
