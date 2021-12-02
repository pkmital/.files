//
//  DGArpeggiator.hpp
//  DINBlossom
//
//  Created by Karl Scholz on 7/7/17.
//  Copyright © 2017 Data Garden
//
/*
 
DGArpeggiator is a classic arpeggiator with a note map on the output.
 
Its usage follows the conventions laid out in BSCommonModules.h, DGVoices.h, DGWavetableSynth.h, etc:
 
A struct holds state
 An init method initializes the state
 A do method updates the state.
 
 Arpeggiators are owned by DGVoices and pass their MIDI output direction to the DGWavetableSynths in each voice.s
 
 See the do methods of the DGVoices for details
 
*/

#ifndef DGArpeggiator_h
#define DGArpeggiator_h


#include <stdio.h>
#include <stdint.h>
#import "BSCommonModules.h"
#import "DGWavetableSynth.h"
#import "DGOscillatorSynth.h"

typedef struct{
    
    //store values notes in scale (0 - 11)
    int notes[12];
    int transpose;
    int post_map_transpose;
    
    int lowest_note;
    int note_range;
    
}DGNoteMap;

DGNoteMap * DGNoteMap_mapForRootKeyAndScale(BSRootKey key, BSMusicalScale scale);

typedef enum{
    DGArpeggiatorModeUp,
    DGArpeggiatorModeDown,
    DGArpeggiatorModeUpDown,
    DGArpeggiatorModeRandom,
    DGArpeggiatorModeAsPlayed,
}DGArpeggiatorMode;

typedef struct {
    bool initialized;//used to tell if we should copy transpose info and delete old note maps
    int sequence[10000];
    int sequence_length;
    int sequence_step_counter;
    
    bool  includedNotes[128];
    
    //instead of using a linked list, each entry in this
    //array will contain the next note
    int  includedNotesInOrder[128];

    float sample_rate;
    int   note_range;
    DGArpeggiatorMode mode;
    float tempo;
    float subdivision;//16 = 16th notes, 0.5 = half notes
    
    float samples_per_step;
    uint64_t sample_counter;
    
    DGNoteMap * noteMap;
    
}DGArpeggiator;

void DGArpeggiator_init(DGArpeggiator * arpeggiator, float sample_rate, int note_range, DGArpeggiatorMode mode, float tempo, float subdivision, DGNoteMap * noteMap);
void DGArpeggiator_clear(DGArpeggiator * arpeggiator);
void DGArpeggiator_updateRealtimeParameters(DGArpeggiator * arpeggiator, float tempo, int lowest_note, int note_range, int trans_pre, int trans_post, float subdivision);
void DGArpeggiator_do(DGArpeggiator * arpeggiator, DGWavetableSynth * synth);
void DGArpeggiator_do(DGArpeggiator * arpeggiator, DGOscillatorSynth * synth);
void DGArpeggiator_midi(DGArpeggiator * arpeggiator, BSMidiMessage * message);
void DGArpeggiator_set_tempo(DGArpeggiator * arpeggiator, float tempo);
void DGArpeggiator_set_subdivision(DGArpeggiator * arpeggiator, float subdivision);

void DGArpeggiator_flush(DGArpeggiator * arpeggiator);

void DGArpeggiator_addNoteToIncludedNotesInOrder(DGArpeggiator * arpeggiator, int note);
void DGArpeggiator_removeNoteFromIncludedNotesInOrder(DGArpeggiator * arpeggiator, int note);
void DGArpeggiator_cleanIncludedNotesInOrder(DGArpeggiator * arpeggiator);

#endif /* DGArpeggiator_hpp */
