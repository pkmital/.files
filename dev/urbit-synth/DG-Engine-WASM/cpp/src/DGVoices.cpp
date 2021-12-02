//
//  DGVoiceManager.m
//  DINBlossom
//
//  Created by Karl Scholz on 7/7/17.
//  Copyright © 2017 Data Garden
//

#import "DGVoices.h"
#import "DGTuningParameters.h"
#import "DGArpeggiator.h"
#import "json.hpp"

#ifdef IOS
#import "DGEngineManager-C-Interface.h"
#endif

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#pragma mark - DGVoice1 C Functions

DGVoiceManager::DGVoiceManager(){
    //init code
}

DGVoiceManager::~DGVoiceManager(){
    //delete code
}


#pragma mark - DGVoice functions

//Glassy

void DGVoice_init(DGVoice * voice, float sample_rate, DGAudioGraph * graph, int voiceNumber){
    
    voice->voiceNumber = voiceNumber;
    voice->active = false;
    
    DGEngineState * state = graph->state;
    
    DGNoteMap * note_map = DGNoteMap_mapForRootKeyAndScale(state->rootKey, state->musicalScale);
    note_map->transpose = state->voices[voiceNumber].voice_tranpose_pre_note_map;
    note_map->post_map_transpose = state->voices[voiceNumber].voice_tranpose_post_note_map;
    note_map->lowest_note = state->voices[voiceNumber].voice_lowest_note;
    note_map->note_range = state->voices[voiceNumber].voice_note_range;
    
    voice->lp_filter = new BSFilter;
    BSFilter_init(voice->lp_filter, BSFilterTypeLowPass, 0.9, 0.0);
    
    voice->hp_filter = new BSFilter;
    BSFilter_init(voice->hp_filter, BSFilterTypeHighPass, 0.7, 0.0);
    
    voice->arpeggiator = new DGArpeggiator;
    
    int last_recognized_cc_value = 0;
    DGArpeggiator_init(voice->arpeggiator, sample_rate, 1, DGArpeggiatorModeAsPlayed, state->master_tempo, graph->rangeMaps[BSParameterRangeMapDestination_Voice1ArpSubdivision + voiceNumber].values[last_recognized_cc_value], note_map);
    
    voice->delayResonator = new DGResonator;
    int step_numbers2[5] = { state->voices[voiceNumber].voice_delay_time_step_1, state->voices[voiceNumber].voice_delay_time_step_2, state->voices[voiceNumber].voice_delay_time_step_3 };
    float feedbacks2[5] = { state->voices[voiceNumber].voice_delay_feedback_1, state->voices[voiceNumber].voice_delay_feedback_2, state->voices[voiceNumber].voice_delay_feedback_3 };
    DGResonator_initAsTempoDelay(voice->delayResonator, sample_rate, 3, state->master_tempo, step_numbers2, feedbacks2);
    
    voice->outputVCA = new DGVCA;
    DGVCA_init(voice->outputVCA, kDEFAULT_SMOOTHING_RATE, 0.0);
    
}

void DGVoice_clear(DGVoice * voice){
    
    DGWavetableSynth_clear(voice->synth);
    
    DGResonator_clearAsTempoDelay(voice->delayResonator);
    
    DGArpeggiator_clear(voice->arpeggiator);
    delete voice->arpeggiator;
    
    DGVCA_clear(voice->outputVCA);
    delete voice->outputVCA;
    
    delete voice->hp_filter;
    delete voice->lp_filter;
    
}

void DGVoice_do(DGVoice * voice, float * input, float * output, DGEngineState * state){
    if(voice->active){
        float signal[2] = {0.0, 0.0};
        
        DGArpeggiator_do(voice->arpeggiator, voice->synth);
        DGWavetableSynth_do(voice->synth, signal);


        
        for(int chan = 0; chan < 2; chan++){
            signal[chan] = signal[chan] * 0.5;
        }
        
        //BSFilter_do(voice->hp_filter, signal, signal);
        //BSFilter_do(voice->lp_filter, signal, signal);




        float from_delay[2] = {0.0, 0.0};
        DGResonator_do(voice->delayResonator, signal, from_delay);
        
        for(int chan = 0; chan < 2; chan++){
            signal[chan] = signal[chan] * state->voices[voice->voiceNumber].voice_delay_dry + from_delay[chan] * state->voices[voice->voiceNumber].voice_delay_wet;
        }


        //    for(int chan = 0; chan < 2; chan++){
        //        output[chan] = signal[chan];
        //    }
        DGVCA_do(voice->outputVCA, signal, output);


    }
}


#pragma mark - DGVoice1Effects C Functions

void DGVoice1Effects_init(DGVoice1Effects * voice1Effects, float sample_rate){
    
    
    voice1Effects->inputVCA = new DGVCA;
    DGVCA_init(voice1Effects->inputVCA, kDEFAULT_SMOOTHING_RATE, 0.0);
    
    //initialize bit crusher
    voice1Effects->crusher = new DGBitCrusher;
    DGBitCrusher_init(voice1Effects->crusher, 2, 8);

    voice1Effects->filter = new BSFilter;
    BSFilter_init(voice1Effects->filter, BSFilterTypeLowPass, 0.5, 0.2);
    //initialize resonator
    voice1Effects->resonator = new DGResonator;
    float note_numbers[5] = { 60.0f, 72.0f, 58.0f, 48.0f, 55.0f };
    float feedbacks[5] = { 0.90, 0.90, 0.95, 0.95, 0.9 };
    float pans[5] = { -0.9, 0.8, -0.7, 0.5, -0.6 };
    DGResonator_init(voice1Effects->resonator, sample_rate, 5, note_numbers, feedbacks, pans);
    

    
}

void DGVoice1Effects_clear(DGVoice1Effects * voice1Effects){
    
    DGVCA_clear(voice1Effects->inputVCA);
    delete voice1Effects->inputVCA;
    delete voice1Effects->crusher;
    delete voice1Effects->filter;
    
    DGResonator_clear(voice1Effects->resonator);
    delete voice1Effects->resonator;

}

void DGVoice1Effects_do(DGVoice1Effects * voice1Effects, float * input, float * output){
    
    float signal[2] = {0.0f, 0.0f};
    float to_vca[2] = {0.0f, 0.0f};
    for(int chan = 0; chan < 2; chan++){
        to_vca[chan] = input[chan] * 0.2;
    }
        
    DGVCA_do(voice1Effects->inputVCA, to_vca, signal);
    
    for(int chan = 0; chan < 2; chan++){
        signal[chan] = signal[chan] * 20.0f;
    }
    
    DGBitCrusher_do(voice1Effects->crusher, signal, signal);
    for(int chan = 0; chan < 2; chan++){
        signal[chan] = signal[chan] / 2.0f;
    }
    BSFilter_do(voice1Effects->filter, signal, signal);
    DGResonator_do(voice1Effects->resonator, signal, signal);
    
    for(int chan = 0; chan < 2; chan++){
        signal[chan] = signal[chan] / 18.0f;
        output[chan] = signal[chan];
    }
}

#pragma mark - DGBusA C Functions

void DGBusA_init(DGBusA * voice, float sample_rate){
    
    voice->freqVCA = new DGVCA;
    DGVCA_init(voice->freqVCA, kDEFAULT_SMOOTHING_RATE, 0.0);

    voice->hp_filter = new BSFilter;
    BSFilter_init(voice->hp_filter, BSFilterTypeHighPass, 0.2, 0.0);
    
    voice->lp_filter = new BSFilter;
    BSFilter_init(voice->lp_filter, BSFilterTypeLowPass, 0.6, 0.0);

}

void DGBusA_clear(DGBusA * voice){
    
    delete voice->freqVCA;
    delete voice->hp_filter;
    delete voice->lp_filter;

}


void DGBusA_do(DGBusA * voice, float * input, float * output){
    
    //hacking VCA to process constant voltage
    float freq[2] = {1.0, 1.0};
    DGVCA_do(voice->freqVCA, freq, freq);
    
    float freqOut = freq[0];
    
    voice->hp_filter->cutoff = freqOut - 0.1;
    voice->lp_filter->cutoff = freqOut + 0.1;
    
    float hp_signal[2] = {0, 0};
    BSFilter_do(voice->hp_filter, input, hp_signal);
    
    float lp_signal[2] = {0, 0};
    BSFilter_do(voice->lp_filter, hp_signal, lp_signal);
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = lp_signal[chan];
    }
}


#pragma mark - DGBusB C Functions

void DGBusB_init(DGBusB * voice, float sample_rate){
    
    voice->freqVCA = new DGVCA;
    DGVCA_init(voice->freqVCA, kDEFAULT_SMOOTHING_RATE, 0.0);
    
    voice->hp_filter = new BSFilter;
    BSFilter_init(voice->hp_filter, BSFilterTypeHighPass, 0.2, 0.0);
    
    voice->lp_filter = new BSFilter;
    BSFilter_init(voice->lp_filter, BSFilterTypeLowPass, 0.6, 0.0);
    
}

void DGBusB_clear(DGBusB * voice){
    
    delete voice->freqVCA;
    delete voice->hp_filter;
    delete voice->lp_filter;
    
}
void DGBusB_do(DGBusB * voice, float * input, float * output){

    
    float hp_signal[2] = {0, 0};
    BSFilter_do(voice->hp_filter, input, hp_signal);
    
    float lp_signal[2] = {0, 0};
    BSFilter_do(voice->lp_filter, hp_signal, lp_signal);
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = lp_signal[chan];
    }

}


#pragma mark - DGOutputEffects C Functions

void DGOutputEffects_updateReverb(DGOutputEffects * output_effects,nlohmann::json reverbJSON, float sample_rate){
    
    /*
    for(int chan = 0; chan < 2; chan++){
        gverb_free(output_effects->static_reverb[chan]);//tail level
    }
    */
    
    if(output_effects->reverbSet){

        output_effects->reverbSet = false;
        std::this_thread::sleep_for (std::chrono::milliseconds(5));
        for(int chan = 0; chan < 2; chan++){
            gverb_free(output_effects->static_reverb[chan]);//tail level
        }
    }
    
    
    for(int chan = 0; chan < 2; chan++){
        output_effects->static_reverb[chan] = gverb_new(
                                                 (int)sample_rate,
                                                 300.0f,//float maxroomsize,
                                                 reverbJSON["ROOM_SIZE"],//roomsize
                                                 reverbJSON["TIME"],//rev time
                                                 reverbJSON["DAMPING"],//damping
                                                 reverbJSON["SPREAD"],//spread
                                                 reverbJSON["BANDWIDTH"],//inputbandwidth
                                                 reverbJSON["EARLY_LEVEL"],//earlylevel
                                                 reverbJSON["TAIL_LEVEL"]);//tail level
    }
    output_effects->reverbSet = true;
}

void DGOutputEffects_init(DGOutputEffects * output_effects, float sample_rate){
    
    output_effects->reverbSet = false;
    //initialize resonator
    output_effects->resonatorVCA = new DGVCA;
    DGVCA_init(output_effects->resonatorVCA, kDEFAULT_SMOOTHING_RATE, 0.0);
    
    output_effects->filter = new BSFilter;
    BSFilter_init(output_effects->filter, BSFilterTypeLowPass, 0.65, 0.3);
    
    output_effects->resonator = new DGResonator;
    float note_numbers[5] = { 48.0f, 55.0f, 36.0f, 72.0f, 60.0f };
    float feedbacks[5] = { 0.7, 0.6, 0.5, 0.3, 0.4 };
    float pans[5] = { -0.6, 0.8, -0.8, 0.6, -0.4 };

    DGResonator_init(output_effects->resonator, sample_rate, 5, note_numbers, feedbacks, pans);
    
    output_effects->delayTimeVCA = new DGVCA;
    DGVCA_init(output_effects->delayTimeVCA, kDEFAULT_SMOOTHING_RATE, 0.139);
    output_effects->modDelay = new DGModDelay;
    DGModDelay_init(output_effects->modDelay, sample_rate, 44100, 0.0, 1.0);
    DGModDelay_setDelayTime(output_effects->modDelay, 0.139);
    
    output_effects->reverbDryVCA = new DGVCA;
    DGVCA_init(output_effects->reverbDryVCA, kDEFAULT_SMOOTHING_RATE, 0.0);
    
    output_effects->reverbSendVCA = new DGVCA;
    DGVCA_init(output_effects->reverbSendVCA, kDEFAULT_SMOOTHING_RATE, 0.0);
    
    /*
     
     for(int chan = 0; chan < 2; chan++){
     output_effects->static_reverb[chan] = gverb_new(
     (int)sample_rate,
     50.f,//float maxroomsize,
     30.0f,//roomsize
     5.0f,//rev time
     0.1f,//damping
     1.0f,//spread
     0.2f,//inputbandwidth
     2.0f,//earlylevel
     5.0f);//tail level
     }
    */
    
//default
    
    /*
    for(int chan = 0; chan < 2; chan++){
        output_effects->static_reverb[chan] = gverb_new(
                                                 (int)sample_rate,
                                                 50.f,//float maxroomsize,
                                                 40.0f,//roomsize
                                                 10.0f,//rev time
                                                 0.1f,//damping
                                                 1.0f,//spread
                                                 0.2f,//inputbandwidth
                                                 2.0f,//earlylevel
                                                 5.0f);//tail level
    }
    */
//from internet
/*
    for(int chan = 0; chan < 2; chan++){
        output_effects->static_reverb[chan] = gverb_new(
                                                 (int)sample_rate,
                                                 350.0f,//float maxroomsize,
                                                 300.0f,//roomsize
                                                 30.0f,//rev time
                                                 1.0f,//damping
                                                 1.0f,//spread
                                                 1.0f,//inputbandwidth
                                                 -21.209999f,//earlylevel
                                                 -21.209999f);//tail level
    }
   */
    
//Hall from internet

    for(int chan = 0; chan < 2; chan++){
        output_effects->static_reverb[chan] = gverb_new(
                                                 (int)sample_rate,
                                                 500.0f,//float maxroomsize,
                                                 300.0f,//roomsize
                                                 20.0f,//rev time
                                                 0.5f,//damping
                                                 2.0f,//spread
                                                 0.75f,//inputbandwidth
                                                 -10.0f,//earlylevel
                                                 -30.0f);//tail level
    }
    output_effects->reverbSet = true;

   
/*
Roomsize: 300.0
Reverb time: 30.0
Damping: 1.0
Input bandwith: 1.0
Dry signal level: 0.0
Early reflection level: -21.209999
Tail level: -21.209999
*/
    
    /*Old setting, not stable on OSX
    for(int chan = 0; chan < 2; chan++){
        output_effects->reverb[chan] = gverb_new(
         (int)sample_rate,
         500.f,//float maxroomsize,
         499.0f,//roomsize
         20.0f,//rev time
         0.2f,//damping
         1.0f,//spread
         0.8f,//inputbandwidth
         2.0f,//earlylevel
         5.0f);//tail level
    }
    */

    output_effects->output_hp_filter = new BSFilter;
    BSFilter_init(output_effects->output_hp_filter, BSFilterTypeHighPass, 0.18, 0.0);
    
}

void DGOutputEffects_clear(DGOutputEffects * output_effects){
    
    delete output_effects->resonatorVCA;
    delete output_effects->filter;
    
    DGResonator_clear(output_effects->resonator);
    delete output_effects->resonator;
    
    delete output_effects->delayTimeVCA;
    
    DGModDelay_clear(output_effects->modDelay);
    
    delete output_effects->reverbDryVCA;
    
    delete output_effects->reverbSendVCA;
    
    if(output_effects->reverbSet){
        for(int chan = 0; chan < 2; chan++){
            gverb_free(output_effects->static_reverb[chan]);//tail level
        }
        output_effects->reverbSet = false;
    }
}

void DGOutputEffects_do(DGOutputEffects * output_effects, float * input, float * output, DGEngineState * state){
    
    float signal[2] = {0.0f, 0.0f};
    
    float to_static_reverb_signal[2] = {0.0f, 0.0f};

    for(int chan = 0; chan < 2; chan++){
        signal[chan] = input[chan] * 0.1000;
        to_static_reverb_signal[chan] = signal[chan] * 0.01;
    }
    
    float static_reverb_wet_signal[2] = {0.0f, 0.0f};
    
    float dummy = 0.0;
    //process left reverb
    
    if(output_effects->reverbSet){
        gverb_do(output_effects->static_reverb[0], to_static_reverb_signal[0],
                 &static_reverb_wet_signal[0], &dummy);
        
        //process right reverb
        gverb_do(output_effects->static_reverb[1], to_static_reverb_signal[1],
                 &dummy, &static_reverb_wet_signal[1]);
    }


    //BSFilter_do(output_effects->output_hp_filter, static_reverb_wet_signal, static_reverb_wet_signal);

    for(int chan = 0; chan < 2; chan++) {

        signal[chan] = (signal[chan] * state->master_reverb_dry) + (static_reverb_wet_signal[chan] * state->master_reverb_wet);
        
    }
    
    for(int chan = 0; chan < 2; chan++){
        output[chan] = signal[chan] * 0.5;
    }
    
}




DGVoice * DGVoiceManager::newVoiceWithSampleRate(float sampleRate, DGAudioGraph * graph, int voiceNumber){
    
    DGVoice * voice = new DGVoice;
    
    DGVoice_init(voice, sampleRate, graph, voiceNumber);
    
    return voice;
}

void DGAudioGraph_updateRealtimeParameters(DGAudioGraph * graph, float sample_rate, nlohmann::json json_state){
    
    loadStateFromJSON(json_state["metadata"], graph->state);
    
    DGAudioGraph_setTuningFrequency(graph, graph->state->tuningFrequency);
    
    DGAudioGraph_setKeyScale(graph, graph->state->rootKey, graph->state->musicalScale);
    
    for(int i = 0; i < graph->numberOfVoices; i++){
        
        BSParameterRangeMap_initWithJSON(
                                         &graph->rangeMaps[BSParameterRangeMapDestination_Voice1Gain + i],
                                         json_state["metadata"]["VOICES"][i]["VOICE_GAIN_ACTIVITY_MAP"]);
        
        //if we have an arp range map
        if(json_state["metadata"]["VOICES"][i].find("VOICE_ARP_SUBDIVISION_ACTIVITY_MAP") != json_state["metadata"]["VOICES"][i].end()){
        
            //we have a range map defined
            
        }
        else{
            
            //create a range map for the basic arp rate
            nlohmann::json pointsJSON;
            nlohmann::json point;
            point.push_back(63.0f);
            point.push_back(json_state["metadata"]["VOICES"][i]["VOICE_ARP_SUBDIVISION"]);
            pointsJSON.push_back(point);
            
            json_state["metadata"]["VOICES"][i]["VOICE_ARP_SUBDIVISION_ACTIVITY_MAP"] = pointsJSON;
            
        }
        
        BSParameterRangeMap_initWithJSON(
                                         &graph->rangeMaps[BSParameterRangeMapDestination_Voice1ArpSubdivision + i],
                                         json_state["metadata"]["VOICES"][i]["VOICE_ARP_SUBDIVISION_ACTIVITY_MAP"]);
        
        
        int last_recognized_cc_value = 0;
        
        DGArpeggiator_updateRealtimeParameters(
                                               graph->voices[i]->arpeggiator,
                                               graph->state->master_tempo,
                                               graph->state->voices[i].voice_lowest_note,
                                               graph->state->voices[i].voice_note_range,
                                               graph->state->voices[i].voice_tranpose_pre_note_map,
                                               graph->state->voices[i].voice_tranpose_post_note_map,
                                               graph->rangeMaps[BSParameterRangeMapDestination_Voice1ArpSubdivision + i].values[last_recognized_cc_value]
                                               );
        
    }
}

void DGAudioGraph_updateRealtimeParametersReverb(DGAudioGraph * graph, float sample_rate, nlohmann::json json_state){
    
    DGAudioGraph_updateRealtimeParameters(graph, sample_rate, json_state);
    
    //update reverb here:
    if(json_state["metadata"].find("REVERB") != json_state["metadata"].end()){
        
        DGOutputEffects_updateReverb(graph->outputEffects, json_state["metadata"]["REVERB"], graph->sampleRate);
    }
    
}

void DGAudioGraph_reloadVoice(DGAudioGraph * graph, float sample_rate, int voiceNumber, nlohmann::json json_state){
    
    //ASSUMING VOICE IS ALREADY LOADED
    
    if(voiceNumber >= graph->numberOfVoices){
        return;
    }

    DGWavetableSynthManager * synthManager = new DGWavetableSynthManager(graph->samplePaths);
    
    graph->voices[voiceNumber]->active = false;
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
    DGWavetableSynth_clear(graph->voices[voiceNumber]->synth);
    graph->voices[voiceNumber]->synth = synthManager->newSynthWithSamples(json_state["metadata"]["VOICES"][voiceNumber]["VOICE_SAMPLE"]);
    graph->voices[voiceNumber]->synth->synthId = voiceNumber;
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
    graph->voices[voiceNumber]->active = true;
    
    delete synthManager;
    
    DGAudioGraph_updateRealtimeParameters(graph, sample_rate, json_state);

}

void DGAudioGraph_init(DGAudioGraph * graph, float sample_rate, nlohmann::json json_state, std::map<std::string, std::vector<std::string>> samplePaths){
            
    graph->samplePaths = samplePaths;
    
    DGVoiceManager * voiceManager = new DGVoiceManager();
    
    graph->state = new DGEngineState();
    graph->sampleRate = sample_rate;
    
    graph->state->rootKey = BSRootKeyG;
    graph->state->musicalScale = BSMusicalScalePentatonicMajor;
    //now that arpeggaitors have been created:

    loadStateFromJSON(json_state["metadata"], graph->state);
    
    DGWavetableSynthManager * synthManager = new DGWavetableSynthManager(graph->samplePaths);
    
    graph->numberOfVoices = json_state["metadata"]["VOICES"].size();
    for(int i = 0; i < graph->numberOfVoices; i++){

        graph->voices[i] = voiceManager->newVoiceWithSampleRate(sample_rate, graph, i);
        graph->voices[i]->synth = synthManager->newSynthWithSamples(json_state["metadata"]["VOICES"][i]["VOICE_SAMPLE"]);
        graph->voices[i]->synth->synthId = i;
        graph->voices[i]->active = true;
        
    }


    graph->voice1SendAVCA = new DGVCA;
    DGVCA_init(graph->voice1SendAVCA, kDEFAULT_SMOOTHING_RATE, 0.5);
    
    graph->voice1SendBVCA = new DGVCA;
    DGVCA_init(graph->voice1SendBVCA, kDEFAULT_SMOOTHING_RATE, 0.5);
    

    graph->busA = new DGBusA;
    DGBusA_init(graph->busA, sample_rate);
    
    graph->busB = new DGBusB;
    DGBusB_init(graph->busB, sample_rate);

    graph->outputEffects = new DGOutputEffects;
    DGOutputEffects_init(graph->outputEffects, sample_rate);

    graph->modDelay = new DGModDelay;
    DGModDelay_init(graph->modDelay, sample_rate, 100, 0.0, 1.0);
    DGModDelay_setDelayTime(graph->modDelay, 0.01);
    

    
    //sets arp settings and range maps
    DGAudioGraph_updateRealtimeParametersReverb(graph, sample_rate, json_state);
    

    DGAudioGraph_setKeyScale(graph, graph->state->rootKey, graph->state->musicalScale);
    
    //init multimode as off
    graph->multimode_enabled = false;
    for(int i = 0; i < kMaxNumberOfVoices; i++){
        //default is -1 which whill never be assigned to a real device
        graph->multimodeAssignments[i] = -1;
    }

    delete voiceManager;
    delete synthManager;
}

void DGAudioGraph_clear(DGAudioGraph* graph){
    
    delete graph->state;
    
    for(int i = 0; i < graph->numberOfVoices; i++){
        DGVoice_clear(graph->voices[i]);
        delete graph->voices[i];
    }
    
    DGVCA_clear(graph->voice1SendAVCA);
    delete graph->voice1SendAVCA;
    
    DGVCA_clear(graph->voice1SendBVCA);
    delete graph->voice1SendBVCA;
    
    DGBusA_clear(graph->busA);
    delete graph->busA;
    
    DGBusB_clear(graph->busB);
    delete graph->busB;

    DGOutputEffects_clear(graph->outputEffects);
    delete graph->outputEffects;

    DGModDelay_clear(graph->modDelay);
    delete graph->modDelay;
    
}


void DGAudioGraph_do(DGAudioGraph * graph, float * input, float * output){



    float v1o[2] = {0.0f, 0.0f};
    DGVoice_do(graph->voices[0], input, v1o, graph->state);

    float v1sao[2] = {0.0f, 0.0f};
    DGVCA_do(graph->voice1SendAVCA, v1o, v1sao);
    
    float v1sbo[2] = {0.0f, 0.0f};
    DGVCA_do(graph->voice1SendBVCA, v1o, v1sbo);
    
    float v2o[2] = {0.0f, 0.0f};
    float v3o[2] = {0.0f, 0.0f};
    float v4o[2] = {0.0f, 0.0f};

    if(graph->numberOfVoices >= 2){
        DGVoice_do(graph->voices[1], input, v2o, graph->state);
    }
    if(graph->numberOfVoices >= 3){
        DGVoice_do(graph->voices[2], input, v3o, graph->state);
    }
    if(graph->numberOfVoices >= 4){
        DGVoice_do(graph->voices[3], input, v4o, graph->state);
    }


    //Calculating Bus Input
    float busAInput[2] = {0.0f, 0.0f};
    for(int i = 0; i < 2; i++){
        //busAInput[i] += v1sao[i];
        //other sends to bus A get summed here
    }
    float busAOutput[2] = {0.0f, 0.0f};
    //DGBusA_do(graph->busA, busAInput, busAOutput);
    
    //Calculating Bus Input
    float busBInput[2] = {0.0f, 0.0f};
    for(int i = 0; i < 2; i++){
        //busBInput[i] += v1sbo[i];
        //other sends to bus B get summed here
    }
    float busBOutput[2] = {0.0f, 0.0f};
    //DGBusB_do(graph->busB, busBInput, busBOutput);

    //Summing


        
    float v1gain = graph->state->voices[0].voice_gain;
    float v2gain = 0.0;//0.5;
    float v3gain = 0.0;
    float v4gain = 0.0;
    
    
    if(graph->numberOfVoices >= 2){
        v2gain = graph->state->voices[1].voice_gain;//0.5;
    }
    if(graph->numberOfVoices >= 3){
        v3gain = graph->state->voices[2].voice_gain;
    }
    if(graph->numberOfVoices >= 4){
        v4gain = graph->state->voices[3].voice_gain;
    }
    //float v5gain = graph->state->voice_5_gain;;//0.01;
    //float v6gain = graph->state->voice_6_gain;;//0.01;
    
    float signal[2] = {0.0f, 0.0f};
    for(int i = 0; i < 2; i++){
        signal[i] += v1o[i] * v1gain;
        
        /*
        if(v1o[i] > 0.9999 || v1o[i] < -0.9999){
            std::cout << "Clipping v1o channel " << i << " with value " << v1o[i] << std::endl;
        }
        */
        signal[i] += v2o[i] * v2gain;
        
        /*
        if(v2o[i] > 0.9999 || v2o[i] < -0.9999){
            std::cout << "Clipping v2o channel " << i << " with value " << v2o[i] << std::endl;
        }
        */
        signal[i] += v3o[i] * v3gain;
        
        /*
        if(v3o[i] > 0.9999 || v3o[i] < -0.9999){
            std::cout << "Clipping v3o channel " << i << " with value " << v3o[i] << std::endl;
        }
        */
        signal[i] += v4o[i] * v4gain;
        /*
        if(v4o[i] > 0.9999 || v4o[i] < -0.9999){
            std::cout << "Clipping v4o channel " << i << " with value " << v4o[i] << std::endl;
        }
        */
        /*
        signal[i] += v5o[i] * v5gain;
        if(v5o[i] > 0.9999 || v5o[i] < -0.9999){
            std::cout << "Clipping v5o channel " << i << " with value " << v5o[i] << std::endl;
        }
        
        signal[i] += v6o[i] * v6gain;
        if(v6o[i] > 0.9999 || v6o[i] < -0.9999){
            std::cout << "Clipping v6o channel " << i << " with value " << v6o[i] << std::endl;
        }
        */
        
        /*
        signal[i] += busAOutput[i] * graph->state->bus_a_return_gain;;
        if(busAOutput[i] > 0.9999 || busAOutput[i] < -0.9999){
            std::cout << "Clipping busAOutput channel " << i << " with value " << busAOutput[i] << std::endl;
        }
        
        signal[i] += busBOutput[i] * graph->state->bus_b_return_gain;
        if(busBOutput[i] > 0.9999 || busBOutput[i] < -0.9999){
            std::cout << "Clipping busBOutput channel " << i << " with value " << busBOutput[i] << std::endl;
        }
         */
    }


    
    DGOutputEffects_do(graph->outputEffects, signal, signal, graph->state);

    float afterDelay[2] = {0.0f, 0.0f};
    //DGModDelay_do(graph->modDelay, signal, afterDelay);
    
    //printing
    for(int i = 0; i < 2; i++){
        
        afterDelay[i] = signal[i];
        output[i] = afterDelay[i] * graph->state->master_gain * 20;
        
        if(output[i] > 0.9999 || output[i] < -0.9999){
            std::cout << "Clipping output channel " << i << " with value " << output[i] << std::endl;
        }
    }


    
    double bpm = 120.0f;
    double sampleRate = graph->sampleRate;
    //double samplesPerBeat = (sampleRate * 60) / bpm;
    double beatsPerSample = bpm / (sampleRate * 60);
    
    graph->beatstamp += beatsPerSample;
    
}

void DGAudioGraph_midi(DGAudioGraph * graph, BSMidiMessage * m){
    //Pipes MIDI to interface and streamer functions in Objective C
    
    //stripping channel info from messages
    if(m->message[0] >= 176 && m->message[0] < 176 + 16){
        m->message[0] = 176;
        m->message[1] = 80;

    }
    else if(m->message[0] >= 144 && m->message[0] <= 144 + 16){
        m->message[0] = 144;
    }
    if(m->message[0] >= 128 && m->message[0] < 128 + 16){
        m->message[0] = 128;
    }
    
    if(!graph->playbackEnabled){
        return;
    }
    
    m->beatstamp = graph->beatstamp;
#ifdef IOS
    midiBusEvent(*m);
#endif
    //if multimode - address these separately
    for(int i = 0; i < graph->numberOfVoices; i++){
        if(graph->multimode_enabled){
            if(m->deviceMultimodeID == graph->multimodeAssignments[i]){
                DGArpeggiator_midi(graph->voices[i]->arpeggiator, m);
            }
        }
        else{
            DGArpeggiator_midi(graph->voices[i]->arpeggiator, m);
        }
    }

    //MIDI CC
    if(m->message[0] == 0xB0){
        
        
        int cc_number = m->message[1];
        int cc_value = m->message[2];
        

        float normalized_cc = (float)cc_value / 127.0f;
        
        if(cc_number == 74){
            float tempo = 20 + (normalized_cc * 169.0f);
            
            for(int i = 0; i < graph->numberOfVoices; i++){
                if(graph->multimode_enabled){
                    if(m->deviceMultimodeID == graph->multimodeAssignments[i]){
                        DGArpeggiator_set_tempo(graph->voices[i]->arpeggiator, tempo);
                    }
                }
                else{
                    DGArpeggiator_set_tempo(graph->voices[i]->arpeggiator, tempo);
                }
            }
        }
        
        if(cc_number == 80){
            
            //random cc value for testing
            //cc_value = (rand() / (RAND_MAX / 127));
            
            //std::cout << "CC " << cc_value << std::endl;

            //Drone Send A 0db -> -25db
            //-24db is 0.0625
            graph->voice1SendAVCA->value = 1 - (normalized_cc * 0.9375f);
            
            //Drone Send B -25 db -> 0db
            graph->voice1SendBVCA->value = 0.0625 + (normalized_cc * 0.9375f);
            
            
            for(int i = 0; i < graph->numberOfVoices; i++){
                
                bool shouldPayAttentionToThisMessage = false;
                
                
                if(graph->multimode_enabled){
                    if(m->deviceMultimodeID == graph->multimodeAssignments[i]){
                        shouldPayAttentionToThisMessage = true;
                    }
                }
                else{
                    shouldPayAttentionToThisMessage = true;
                }
                
                if(shouldPayAttentionToThisMessage){
                    
                    //update volume of instrument based on CC
                    graph->voices[i]->outputVCA->value = graph->rangeMaps[BSParameterRangeMapDestination_Voice1Gain + i].values[cc_value];
                    
                    DGArpeggiator_set_subdivision(graph->voices[i]->arpeggiator, graph->rangeMaps[BSParameterRangeMapDestination_Voice1ArpSubdivision + i].values[cc_value]);

                }
                

            }





            
            //Send A 451-1.49khz
            graph->busA->freqVCA->value = 0.4 + (normalized_cc * 0.6);

            //Master Delay 1 - 6ms
            DGModDelay_setDelayTime(graph->modDelay, 0.1 + (0.001 + (normalized_cc * 0.005)));
            
        }
    }
}



void DGAudioGraph_setTuningFrequency(DGAudioGraph * graph, float frequency){
    for(int i = 0; i < graph->numberOfVoices; i++){
        DGWavetableSynth_setTuningFrequency(graph->voices[i]->synth, frequency);
    }
}

void DGAudioGraph_enableMultimode(DGAudioGraph * graph, bool enable){
    graph->multimode_enabled = enable;
}

void DGAudioGraph_setMultimodeIDForVoice(DGAudioGraph * graph, int multimode_id, int voice){
    if(voice < kMaxNumberOfVoices){
        graph->multimodeAssignments[voice] = multimode_id;
    }
}

int DGAudioGraph_getMultimodeIDForVoice(DGAudioGraph * graph, int voice){
    return graph->multimodeAssignments[voice];
}


void DGAudioGraph_updateNoteMaps(DGAudioGraph * graph){
    
    
    
    for(int i = 0; i < graph->numberOfVoices; i++){
        
        //std::cout << "Updating noteMap for voice " << i << std::endl;
        
        graph->voices[i]->arpeggiator->noteMap = DGNoteMap_mapForRootKeyAndScale(graph->state->rootKey, graph->state->musicalScale);
        graph->voices[i]->arpeggiator->noteMap->transpose = graph->state->voices[i].voice_tranpose_pre_note_map;
        graph->voices[i]->arpeggiator->noteMap->post_map_transpose = graph->state->voices[i].voice_tranpose_post_note_map;
        graph->voices[i]->arpeggiator->noteMap->lowest_note = graph->state->voices[i].voice_lowest_note;
        graph->voices[i]->arpeggiator->noteMap->note_range = graph->state->voices[i].voice_note_range;
    }
}

void DGAudioGraph_setKeyScale(DGAudioGraph* graph, BSRootKey key, BSMusicalScale scale){
    
    graph->state->rootKey = key;
    graph->state->musicalScale = scale;
    DGAudioGraph_updateNoteMaps(graph);
}

void DGAudioGraph_setRootKey(DGAudioGraph* graph, BSRootKey key){
    
    graph->state->rootKey = key;
    
    DGAudioGraph_updateNoteMaps(graph);
    //update note maps
}
void DGAudioGraph_setScale(DGAudioGraph* graph, BSMusicalScale scale){
    graph->state->musicalScale = scale;
    
    DGAudioGraph_updateNoteMaps(graph);
    //update note maps
}

void DGAudioGraph_incrementRootKey(DGAudioGraph* graph, int direction){
    int newKey = (int)graph->state->rootKey + direction;
    if(newKey < 0){
        newKey += BSRootKeyCount;
    }
    newKey %= BSRootKeyCount;
    
    DGAudioGraph_setRootKey(graph, (BSRootKey)newKey);
}
void DGAudioGraph_incrementScale(DGAudioGraph* graph, int direction){
    int newScale = (int)graph->state->musicalScale + direction;
    if(newScale < 0){
        newScale += BSMusicalScaleCount;
    }
    newScale %= BSMusicalScaleCount;
    
    DGAudioGraph_setScale(graph, (BSMusicalScale)newScale);
}


void loadStateFromJSON(nlohmann::json json_data, DGEngineState * state){
    
    //std::cout << "Load state from JSON" << "\n" << std::endl;
    if(!json_data["NAME"].is_null()){
        
        std::string s = json_data["NAME"];
        char cstring[s.size() + 1];
        strcpy(cstring, s.c_str());
        state->name = strdup(cstring);
        
    }
    else{
        state->name = strdup("No Name");
    }
    
    if(!json_data["MASTER_TEMPO"].is_null()){
        state->master_tempo = json_data["MASTER_TEMPO"];
        //std::cout << "Loaded master tempo " << state->master_tempo << "\n" << std::endl;
    }
    
    if(!json_data["MASTER_GAIN"].is_null()){
        state->master_gain = json_data["MASTER_GAIN"];
        //std::cout << "Loaded master gain " << state->master_gain << "\n" << std::endl;
    }
    
    if(!json_data["MASTER_REVERB_DRY"].is_null()){
        state->master_reverb_dry = json_data["MASTER_REVERB_DRY"];
    }
    
    if(!json_data["MASTER_REVERB_WET"].is_null()){
        state->master_reverb_wet = json_data["MASTER_REVERB_WET"];
    }
    
    if(!json_data["BUS_A_RETURN_GAIN"].is_null()){
        state->bus_a_return_gain = json_data["BUS_A_RETURN_GAIN"];
    }
    if(!json_data["BUS_B_RETURN_GAIN"].is_null()){
        state->bus_b_return_gain = json_data["BUS_B_RETURN_GAIN"];
    }
    
    if(!json_data["TUNING"].is_null()){
        
        if(!json_data["TUNING"]["root"].is_null()){
            std::string s = json_data["TUNING"]["root"];
            char cstring[s.size() + 1];
            strcpy(cstring, s.c_str());
            state->rootKey = BSRootKey_rootKeyFromString(strdup(cstring));
        }
        
        if(!json_data["TUNING"]["scale"].is_null()){
            std::string s = json_data["TUNING"]["scale"];
            char cstring[s.size() + 1];
            strcpy(cstring, s.c_str());
            state->musicalScale = BSMusicalScale_scaleFromString(strdup(cstring));
        }
        
        if(!json_data["TUNING"]["frequency"].is_null()){
            state->tuningFrequency = (float)json_data["TUNING"]["frequency"];
        }

    }
    for(int i = 0; i < 4; i++){
        
        nlohmann::json voiceJSON = json_data["VOICES"][i];
        
        if(!voiceJSON["VOICE_GAIN"].is_null()){
            state->voices[i].voice_gain = voiceJSON["VOICE_GAIN"];
        }
        
        if(!voiceJSON["VOICE_TRANSPOSE_PRE_NOTE_MAP"].is_null()){
            state->voices[i].voice_tranpose_pre_note_map = voiceJSON["VOICE_TRANSPOSE_PRE_NOTE_MAP"];
        }
        
        if(!voiceJSON["VOICE_TRANSPOSE_POST_NOTE_MAP"].is_null()){
            state->voices[i].voice_tranpose_post_note_map = voiceJSON["VOICE_TRANSPOSE_POST_NOTE_MAP"];
        }
        
        if(!voiceJSON["VOICE_LOWEST_NOTE"].is_null()){
            state->voices[i].voice_lowest_note = voiceJSON["VOICE_LOWEST_NOTE"];
        }
        
        if(!voiceJSON["VOICE_NOTE_RANGE"].is_null()){
            state->voices[i].voice_note_range = voiceJSON["VOICE_NOTE_RANGE"];
        }
        
        if(!voiceJSON["VOICE_DELAY_DRY"].is_null()){
            state->voices[i].voice_delay_dry = voiceJSON["VOICE_DELAY_DRY"];
        }
        
        if(!voiceJSON["VOICE_DELAY_WET"].is_null()){
            state->voices[i].voice_delay_wet = voiceJSON["VOICE_DELAY_WET"];
        }
        
        if(!voiceJSON["VOICE_DELAY_TIME_STEP_1"].is_null()){
            state->voices[i].voice_delay_time_step_1 = voiceJSON["VOICE_DELAY_TIME_STEP_1"];
        }
        
        if(!voiceJSON["VOICE_DELAY_TIME_STEP_2"].is_null()){
            state->voices[i].voice_delay_time_step_2 = voiceJSON["VOICE_DELAY_TIME_STEP_2"];
        }
        
        if(!voiceJSON["VOICE_DELAY_TIME_STEP_3"].is_null()){
            state->voices[i].voice_delay_time_step_3 = voiceJSON["VOICE_DELAY_TIME_STEP_3"];
        }
        
        if(!voiceJSON["VOICE_DELAY_FEEDBACK_1"].is_null()){
            state->voices[i].voice_delay_feedback_1 = voiceJSON["VOICE_DELAY_FEEDBACK_1"];
        }
        
        if(!voiceJSON["VOICE_DELAY_FEEDBACK_2"].is_null()){
            state->voices[i].voice_delay_feedback_2 = voiceJSON["VOICE_DELAY_FEEDBACK_2"];
        }
        
        if(!voiceJSON["VOICE_DELAY_FEEDBACK_3"].is_null()){
            state->voices[i].voice_delay_feedback_3 = voiceJSON["VOICE_DELAY_FEEDBACK_3"];
        }
        
    }

}

