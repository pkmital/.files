//
//  Datagarden.cpp
//  DatagardenSandbox_iOS
//
//  Created by Karl Scholz on 10/13/18.
//  Copyright © 2018 Data Garden. All rights reserved.
//
#include "Datagarden.h"
#include <iostream>
#include <fstream>


BSMidiMessage midiMessageForCC(int ccnumber, int value){
    BSMidiMessage m;
    m.message[0] = 0xB0;
    m.message[1] = ccnumber;
    m.message[2] = value;
    m.scheduled = false;
    return m;
}

BSMidiMessage midiMessageForNoteOn(int note){
    BSMidiMessage m;
    m.message[0] = 0x90;
    m.message[1] = note;
    m.message[2] = 127;
    m.scheduled = false;
    return m;
}

BSMidiMessage midiMessageForNoteOff(int note){
    BSMidiMessage m;
    m.message[0] = 0x80;
    m.message[1] = note;
    m.message[2] = 0;
    m.scheduled = false;
    return m;
}

void DGAudioEngineInputProcessor_init(DGAudioEngineInputProcessor * processor){
    
    processor->IBIRollingAverageSmoother = new DGParameterSmoother;
    DGParameterSmoother_init(processor->IBIRollingAverageSmoother, 0.99);//set the rate between 0.00001 and 0.9999, higher is slower

    for(int i = 0; i < 10; i++){
        
        processor->last10notes[i] = 0;
        
    }
    processor->lastIBIValue = 0;
    processor->last10noteIndex = 0 ;
    processor->noteThinning = 8;//higher number dampens notes
    processor->noteShift = 0;
    
}

void DGAudioEngineMIDIScheduler_init(DGAudioEngineMIDIScheduler * scheduler){
    
    scheduler->messagesLength = 200;
    scheduler->messages = new BSMidiMessage[scheduler->messagesLength];
    
    for(int i = 0; i < scheduler->messagesLength; i++){
        scheduler->messages[i].scheduled = false;
    }

}
void DGAudioEngineMIDIScheduler_do(DGAudioEngineMIDIScheduler * scheduler, DGAudioGraph * graph, double currentTime){
    for(int i = 0; i < scheduler->messagesLength; i++){
        if(scheduler->messages[i].scheduled && currentTime > scheduler->messages[i].timestamp){
            BSMidiMessage m = scheduler->messages[i];
            //std::cout << "triggering midi message" << std::endl;
            DGAudioGraph_midi(graph, &m);
            scheduler->messages[i].scheduled = false;
        }
    }
}
void DGAudioEngineMIDIScheduler_scheduleMessage(DGAudioEngineMIDIScheduler * scheduler, BSMidiMessage mi){
    
    mi.scheduled = false;
    for(int i = 0; i < scheduler->messagesLength; i++){
        BSMidiMessage mo = scheduler->messages[i];
        if(!mi.scheduled && !mo.scheduled){
            mi.scheduled = true;
            scheduler->messages[i] = mi;
        }
    }
}

void DGAudioEngineMIDIScheduler_midiFlush(DGAudioEngineMIDIScheduler * scheduler){
    for(int i = 0; i < scheduler->messagesLength; i++){
        scheduler->messages[i].scheduled = false;
    }
}
void DGAudioEngineMIDIScheduler_clear(DGAudioEngineMIDIScheduler * scheduler){
    delete scheduler->messages;
}

DGAudioEngine::DGAudioEngine(){
    //init code
    running = false;
    
}

DGAudioEngine::~DGAudioEngine(){
    //delete code
    
}


void DGAudioEngine::renderAudio(float *audioBuffer, uint32_t numFrames, uint32_t numChannels, double currentTime){
    
    int q = 0;
    
    for (q = 0; q < numFrames; q++) {
        
        if(numChannels > 2){
            
            std::cout << "Greater than 2 channels!!! Please tell Karl. Actual number of channels is " << numChannels << std::endl;

        }
        
        float signal[2] = {0.0, 0.0};
        
        if(running){
            DGAudioEngineMIDIScheduler_do(midiScheduler, graph, currentTime);
            DGAudioGraph_do(graph, signal, signal);
        }
        
        int chan = 0;
        for(chan = 0; chan < numChannels; chan++){
            int bufferIndex = (q * numChannels) + chan;
            
            if(chan < 2){
                audioBuffer[bufferIndex] = signal[chan];
            }
            else{
                audioBuffer[bufferIndex] = 0;
            }
        }
        
        currentTime += 1000.0f / graph->sampleRate;
    }
    
}

void DGAudioEngine::midi(BSMidiMessage *message){
    if(running){
        DGAudioGraph_midi(graph, message);
    }
    else{
                
    }
}

void DGAudioEngine::midiFlush(){
    
    DGAudioEngineMIDIScheduler_midiFlush(midiScheduler);
    
    if(running){
        for(int i = 0; i < 127; i++){
            BSMidiMessage m = midiMessageForNoteOff(i);
            DGAudioGraph_midi(graph, &m);
        }
        BSMidiMessage cc = midiMessageForCC(80, 0);
        DGAudioGraph_midi(graph, &cc);
    }
}

void DGAudioEngine::scheduleMIDIMessage(BSMidiMessage message){
    DGAudioEngineMIDIScheduler_scheduleMessage(midiScheduler, message);
}

nlohmann::json DGAudioEngine::readJSONintoState(std::string filepath){
    
    using json = nlohmann::json;
    json json_state;
    std::ifstream i(filepath);
    i >> json_state;
    

    return json_state;
     
}

void DGAudioEngine::clearBuffers(){
    DGAudioGraph_clear(graph);
    delete graph;
    delete inputProcessor;
    
    DGAudioEngineMIDIScheduler_clear(midiScheduler);
    delete midiScheduler;
}

void DGAudioEngine::initialize(nlohmann::json json_state, std::map<std::string, std::vector<std::string>> samplePaths){
    //passed DMT file - unzip / open to json
    //using json = nlohmann::json;

    float sample_rate = 44100.0f;
    
    graph = new DGAudioGraph;
    
    //set sample paths for graph
        
    DGAudioGraph_init(graph, sample_rate, json_state, samplePaths);
    
    inputProcessor = new DGAudioEngineInputProcessor;
    DGAudioEngineInputProcessor_init(inputProcessor);
    
    midiScheduler = new DGAudioEngineMIDIScheduler;
    DGAudioEngineMIDIScheduler_init(midiScheduler);
}

void DGAudioEngine::switchToDMT(std::string filepath){
    
    using json = nlohmann::json;

    std::ifstream i(filepath);
    json json_state;
    i >> json_state;
    
    float sample_rate = 44100.0f;
    DGAudioGraph_updateRealtimeParametersReverb(graph, sample_rate, json_state);
}

char* DGAudioEngine::currentDMTName(){
    return graph->state->name;
}

char* DGAudioEngine::currentRootKey(){
    return BSRootKey_nameForRootKey(graph->state->rootKey);
}

float DGAudioEngine::currentRootFrequency(){
    return graph->state->tuningFrequency;
}

float DGAudioEngine::currentTempo(){
    return graph->state->master_tempo;
}


char* DGAudioEngine::currentScale(){
    return BSMusicalScale_nameForScale(graph->state->musicalScale);
}

void DGAudioEngine::incrementRootKey(int direction){
    DGAudioGraph_incrementRootKey(graph, direction);
}

void DGAudioEngine::incrementScale(int direction){
    DGAudioGraph_incrementScale(graph, direction);
}

void DGAudioEngine::updateRealtimeParameters(nlohmann::json jsonData){
    //passed DMT file - unzip / open to json
    float sample_rate = 44100.0f;

    DGAudioGraph_updateRealtimeParameters(graph, sample_rate, jsonData);
}

void DGAudioEngine::updateRealtimeParametersReverb(nlohmann::json jsonData){
    //passed DMT file - unzip / open to json
    float sample_rate = 44100.0f;

    DGAudioGraph_updateRealtimeParametersReverb(graph, sample_rate, jsonData);
    
    
}

void DGAudioEngine::reloadVoice(int voiceNumber, nlohmann::json jsonData){
    float sample_rate = 44100.0f;
    DGAudioGraph_reloadVoice(graph, sample_rate, voiceNumber, jsonData);

}

void DGAudioEngine::start(){
    running = true;
}

void DGAudioEngine::pause(){
    running = false;
}

void DGAudioEngine::startPlayback(){
    graph->playbackEnabled = true;
}

void DGAudioEngine::stopPlayback(){
    graph->playbackEnabled = false;
}

bool DGAudioEngine::playbackEnabled(){
    return graph->playbackEnabled;
}

void DGAudioEngine::mute(bool mute){
    graph->muted = mute;
}

bool DGAudioEngine::muted(){
    return graph->muted;
}

void DGAudioEngine::setTuningFrequency(float frequency){
    
    DGAudioGraph_setTuningFrequency(graph, frequency);
    
}

void DGAudioEngine::enableMultimode(bool enable){
    
    DGAudioGraph_enableMultimode(graph, enable);
    
}

void DGAudioEngine::setMultimodeIdForVoice(int multimode_id, int voice){
    DGAudioGraph_setMultimodeIDForVoice(graph, multimode_id, voice);
}
int DGAudioEngine::getMultimodeIdForVoice(int voice){
    return DGAudioGraph_getMultimodeIDForVoice(graph, voice);
}
