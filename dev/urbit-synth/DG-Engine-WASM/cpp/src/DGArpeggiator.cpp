//
//  DGArpeggiator.cpp
//  DINBlossom
//
//  Created by Karl Scholz on 7/7/17.
//  Copyright © 2017 Data Garden
//

#include "DGArpeggiator.h"
#include "BSCommonModules.h"

int DGNoteMap_closestNoteInMap(BSMusicalScaleMap * scaleMap, int note){
    int closestNote = 0;
    
    for(int i = 0; i < scaleMap->notesLength; i++){
        //if distance to note in map is less than closest note
        if(abs(note - scaleMap->notes[i]) < abs(note-closestNote)){
            closestNote = scaleMap->notes[i];
        }
    }
    return closestNote;
    //return position found in scaleMap that is closest to incoming note
}

void DGNoteMap_populateNotesArrayForKeyAndScale(DGNoteMap * map, BSRootKey key, BSMusicalScale scale){
    

    int selectedRoot = BSRootKey_midiNumberForRootKey(key);
    
    //std::cout << "Midi root for key is " << selectedRoot << std::endl;
    
    for(int note = 0; note < 12; note++){
        
        int positionInScale = (note - selectedRoot);
        while (positionInScale < 0){
            positionInScale += 12;
        }
        positionInScale %= 12;
        
        //position in scale is now 0-11 and needs to be mapped to closest note in scale
        BSMusicalScaleMap * scaleMap = BSMusicalScale_mapForScale(scale);
        
        map->notes[note] = (DGNoteMap_closestNoteInMap(scaleMap, positionInScale) + selectedRoot) % 12;
        
        delete scaleMap->notes;
        delete scaleMap;
        //find note in
        
    }
}

DGNoteMap *  DGNoteMap_mapForRootKeyAndScale(BSRootKey key, BSMusicalScale scale){
    DGNoteMap * map = new DGNoteMap;
    DGNoteMap_populateNotesArrayForKeyAndScale(map, key, scale);
    
    /*
    for(int i = 0; i < 12; i++){
        std::cout << "note in map " << map->notes[i] << std::endl;
    }
    */
    map->transpose = 0;
    map->post_map_transpose = 0;
    map->lowest_note = 0;
    map->note_range = 127;
    return map;
}

int DGNoteMap_do(DGNoteMap * map, int unmapped_note){

    int transposed_note = unmapped_note + map->transpose;
    
    while(transposed_note < 12){
        transposed_note += 12;
    }
    
    int note_in_scale = transposed_note % 12;
    int base_octave = transposed_note - note_in_scale;
    
    int note_out = map->notes[note_in_scale] + base_octave;
    
    note_out += map->post_map_transpose;
    
    if(map->note_range <= 0){
        map->note_range = 1;
    }
    
    int highest_note = map->lowest_note + map->note_range;
    
    if(highest_note > 127){
        highest_note = 127;
    }

    if(note_out > highest_note){
        
        while(note_out > highest_note){
            note_out -= 12;
        }
        
    }
    else if(note_out < map->lowest_note){
        
        while(note_out < map->lowest_note){
            note_out += 12;
        }

    }
    
    if(note_out < 0){
        note_out = 0;
    }
    else if(note_out > 127){
        note_out = 127;
    }
    
    
    return note_out;
}

void DGArpeggiator_setNoteMap(DGArpeggiator * arpeggiator, DGNoteMap * map){

    if(arpeggiator->initialized){
        delete arpeggiator->noteMap;
    }

    arpeggiator->noteMap = map;
    
}

void DGArpeggiator_updateRealtimeParameters(DGArpeggiator * arpeggiator, float tempo, int lowest_note, int note_range, int trans_pre, int trans_post, float subdivision){

    arpeggiator->tempo = tempo;
    DGArpeggiator_set_subdivision(arpeggiator, subdivision);//updates tempo cached values
    arpeggiator->noteMap->lowest_note = lowest_note;
    arpeggiator->noteMap->transpose = trans_pre;
    arpeggiator->noteMap->post_map_transpose = trans_post;
    arpeggiator->noteMap->note_range = note_range;

}

void DGArpeggiator_init(DGArpeggiator * arpeggiator, float sample_rate, int note_range, DGArpeggiatorMode mode, float tempo, float subdivision, DGNoteMap * noteMap){

    for(int i = 0; i < 128; i++){
        arpeggiator->includedNotes[i] = false;
        arpeggiator->includedNotesInOrder[i] = -1;
    }
    
    arpeggiator->sample_rate = sample_rate;
    
    arpeggiator->note_range = note_range;
    arpeggiator->mode = mode;
    arpeggiator->subdivision = subdivision;
    
    DGArpeggiator_set_tempo(arpeggiator, tempo);

    arpeggiator->sequence_length = 0;
    arpeggiator->sequence_step_counter = 0;
    arpeggiator->sample_counter = -1;
    
    arpeggiator->noteMap = noteMap;
    arpeggiator->initialized = true;
}

void DGArpeggiator_clear(DGArpeggiator * arpeggiator){
    
    delete arpeggiator->noteMap;
    
}

void DGArpeggiator_flush(DGArpeggiator * arpeggiator){
    
    for(int i = 0; i < 128; i++){
        arpeggiator->includedNotes[i] = false;
        arpeggiator->includedNotesInOrder[i] = -1;
    }
    
    arpeggiator->sequence_length = 0;
    arpeggiator->sequence_step_counter = 0;

}

void DGArpeggiator_set_tempo(DGArpeggiator * arpeggiator, float tempo){
    arpeggiator->tempo = tempo;
    float seconds_per_step = (60.0f * 4.0f / arpeggiator->tempo) / arpeggiator->subdivision;
    arpeggiator->samples_per_step = arpeggiator->sample_rate * seconds_per_step;
}

void DGArpeggiator_set_subdivision(DGArpeggiator * arpeggiator, float subdivision){
    arpeggiator->subdivision = subdivision;
    DGArpeggiator_set_tempo(arpeggiator, arpeggiator->tempo);
}

void DGArpeggiator_render_sequence(DGArpeggiator * arpeggiator){
    
    //std::cout << "DGArpeggiator_render_sequence" << std::endl;
 
    int held_keys[128];
    int number_of_notes = 0;
    
    //get a list of the notes that are held down and count them
    for(int i = 0; i < 128; i++){
        if(arpeggiator->includedNotes[i]){
            held_keys[number_of_notes] = i;
            number_of_notes++;
        }
    }
    
    //std::cout << "DGArpeggiator_render_sequence " << number_of_notes << std::endl;

    
    if(arpeggiator->mode == DGArpeggiatorModeAsPlayed){
        for(int i = 0; i < number_of_notes; i++){
            held_keys[i] = arpeggiator->includedNotesInOrder[i];
            
            //std::cout << "DGArpeggiator_render_sequence adding " << held_keys[i] << std::endl;

        }
    }

    //Rendering sequence
    //arpeggiator up mode
    int sequence_position = 0;
    
    int octave_range = (int)(arpeggiator->note_range / 12);
    if(octave_range == 0) octave_range = 1;
    
    if(arpeggiator->mode == DGArpeggiatorModeUp){
        
        for(int octave = 0; octave < octave_range; octave++){
            for(int note_index = 0; note_index < number_of_notes; note_index++){
                int next_note = held_keys[note_index] + (octave * 12);
                while(next_note > 127){
                    next_note -= 12;
                }
                while(next_note < 0){
                    next_note += 12;
                }
                arpeggiator->sequence[sequence_position] = next_note;
                sequence_position++;
            }
        }
    }
    
    if(arpeggiator->mode == DGArpeggiatorModeAsPlayed){
        for(int octave = 0; octave < octave_range; octave++){
            for(int note_index = 0; note_index < number_of_notes; note_index++){
                int next_note = held_keys[note_index] + (octave * 12);
                while(next_note > 127){
                    next_note -= 12;
                }
                while(next_note < 0){
                    next_note += 12;
                }
                arpeggiator->sequence[sequence_position] = next_note;
                sequence_position++;
            }
        }
    }
    
    arpeggiator->sequence_length = sequence_position;
    
}

int DGArpeggiator_do_local(DGArpeggiator * arpeggiator){
    
    int note_out = -1;
    
    if(arpeggiator->sequence_step_counter >= arpeggiator->sequence_length){
        arpeggiator->sequence_step_counter = 0;
    }
    
    
    if(arpeggiator->sample_counter == 0){
        //trigger
        if(arpeggiator->sequence_length > 0){
                        
            int unmapped_note = arpeggiator->sequence[arpeggiator->sequence_step_counter];
            
            if(unmapped_note > 127 || unmapped_note < 0){
                //NSLog(@"Out of bounds unmapped note: %i", unmapped_note);
                //NSLog(@"Arp info seq length:%i", arpeggiator->sequence_length);
                //NSLog(@"Arp info seq step counter:%i", arpeggiator->sequence_step_counter);
            }
            
            int mapped_note = DGNoteMap_do(arpeggiator->noteMap, unmapped_note);
            
            note_out = mapped_note;
            
            arpeggiator->sequence_step_counter++;
            if(arpeggiator->sequence_step_counter > arpeggiator->sequence_length){
                arpeggiator->sequence_step_counter = 0;
            }
        }
        else{
            //std::cout << "arpeggiator->sequence_length == 0 " << std::endl;

        }
        
    }
    
    arpeggiator->sample_counter++;
    
    if(arpeggiator->sample_counter >= arpeggiator->samples_per_step){
        arpeggiator->sample_counter = 0;
    }
    
    return note_out;
}

void DGArpeggiator_do(DGArpeggiator * arpeggiator, DGWavetableSynth * synth){
    int note = DGArpeggiator_do_local(arpeggiator);
    if(note >= 0 && note <= 127){
        DGWavetableSynth_playNote(synth, note);
    }
}

void DGArpeggiator_do(DGArpeggiator * arpeggiator, DGOscillatorSynth * synth){
    int note = DGArpeggiator_do_local(arpeggiator);
    if(note >= 0){
        DGOscillatorSynth_playNote(synth, note);
    }
}

void DGArpeggiator_note_on(DGArpeggiator * arpeggiator, int note){
    
    //std::cout << "DGArpeggiator_note_on" << note << std::endl;

    arpeggiator->includedNotes[note] = true;
    
    DGArpeggiator_addNoteToIncludedNotesInOrder(arpeggiator, note);
    
    DGArpeggiator_render_sequence(arpeggiator);
    
}
void DGArpeggiator_note_off(DGArpeggiator * arpeggiator, int note){
    
    //std::cout << "DGArpeggiator_note_off" << note << std::endl;
    
    DGArpeggiator_removeNoteFromIncludedNotesInOrder(arpeggiator, note);
    
    arpeggiator->includedNotes[note] = false;
    DGArpeggiator_render_sequence(arpeggiator);
}

void DGArpeggiator_addNoteToIncludedNotesInOrder(DGArpeggiator * arpeggiator, int note){

    DGArpeggiator_removeNoteFromIncludedNotesInOrder(arpeggiator, note);
    
    int index = 0;
    
    while(arpeggiator->includedNotesInOrder[index] >= 0){
        index++;
    }
    
    arpeggiator->includedNotesInOrder[index] = note;
    
}

void DGArpeggiator_removeNoteFromIncludedNotesInOrder(DGArpeggiator * arpeggiator, int note){
    
    for(int i = 0; i < 128; i++){
        if(arpeggiator->includedNotesInOrder[i] == note){
            arpeggiator->includedNotesInOrder[i] = -1;
        }
    }
    
    DGArpeggiator_cleanIncludedNotesInOrder(arpeggiator);
    
}


//turns [0, -1 , -1, 2, 100, 10....] to [0, 2, 100, 10, -1, -1....];
void DGArpeggiator_cleanIncludedNotesInOrder(DGArpeggiator * arpeggiator){
    
    int counter = 0;
    
    for(int i = 0; i < 128; i++){
        
        //if not empty
        if(arpeggiator->includedNotesInOrder[i] >= 0){

            //store note at i
            int temp_note = arpeggiator->includedNotesInOrder[i];
            
            //erase from list
            arpeggiator->includedNotesInOrder[i] = -1;
            
            //write at next position
            arpeggiator->includedNotesInOrder[counter] = temp_note;
            counter++;
        }
        
    }

}
void DGArpeggiator_midi(DGArpeggiator * arpeggiator, BSMidiMessage * message){
    
    //note on
    if(message->message[0] == 0x90){

        if((int)message->message[2] > 0){
            DGArpeggiator_note_on(arpeggiator, (int)message->message[1]);
        }
        else{
            DGArpeggiator_note_off(arpeggiator, (int)message->message[1]);
        }
    }
    else if(message->message[0] == 0x80){
        DGArpeggiator_note_off(arpeggiator, (int)message->message[1]);
    }
    
}
