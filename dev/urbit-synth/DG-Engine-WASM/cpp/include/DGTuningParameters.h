//
//  DGTuningParameters.hpp
//  DGEngineHeartMath
//
//  Created by Karl Scholz on 12/12/18.
//  Copyright © 2018 Data Garden. All rights reserved.
//

#ifndef DGTuningParameters_hpp
#define DGTuningParameters_hpp

#include <stdio.h>
#import "BSCommonModules.h"

#define ENABLE_MIDI_INPUT 1
#define ENABLE_MIDI_OUTPUT 0
#define SHOW_CC80_SLIDER 1

/* TUNING PARAMETERS */

#define kDMT_NAME "Custom Sounds"

#define kMASTER_TEMPO 53.2 //Floating Point number greater than 0.0 (1.0 is unity) //4.0


#define kMASTER_GAIN 35.0 //Floating Point number greater than 0.0 (1.0 is unity) //4.0

#define kMASTER_REVERB_DRY 0.2f
#define kMASTER_REVERB_WET 2.0f

//Drone
#define kVOICE_1_GAIN 0.15//0.0 //Floating Point number greater than 0.0 (1.0 is unity)

//Shimmer
#define kVOICE_2_GAIN 0.1 //Floating Point number greater than 0.0 (1.0 is unity)

//Glassy
#define kVOICE_3_GAIN 0.08 //Floating Point number greater than 0.0 (1.0 is unity)

//Quartet
#define kVOICE_4_GAIN 0.15 //Floating Point number greater than 0.0 (1.0 is unity)

//Down Voice
#define kVOICE_5_GAIN 0.02 //Floating Point number greater than 0.0 (1.0 is unity)

//Up Voice
#define kVOICE_6_GAIN 0.007 //Floating Point number greater than 0.0 (1.0 is unity)

#define kBUS_A_RETURN_GAIN 0.2 //Floating Point number greater than 0.0 (1.0 is unity)
#define kBUS_B_RETURN_GAIN 0.1 //Floating Point number greater than 0.0 (1.0 is unity)

#define kVOICE_1_TRANSPOSE_PRE_NOTE_MAP 24 //Int Value before Mapped to Scale
#define kVOICE_2_TRANSPOSE_PRE_NOTE_MAP 64 //
#define kVOICE_3_TRANSPOSE_PRE_NOTE_MAP 0 //
#define kVOICE_4_TRANSPOSE_PRE_NOTE_MAP 48 //
#define kVOICE_5_TRANSPOSE_PRE_NOTE_MAP -24 //
#define kVOICE_6_TRANSPOSE_PRE_NOTE_MAP -24 //

#define kVOICE_1_TRANSPOSE_POST_NOTE_MAP 2 //Int Value before Mapped to Scale
#define kVOICE_2_TRANSPOSE_POST_NOTE_MAP 0 //
#define kVOICE_3_TRANSPOSE_POST_NOTE_MAP 0 //
#define kVOICE_4_TRANSPOSE_POST_NOTE_MAP 0 //
#define kVOICE_5_TRANSPOSE_POST_NOTE_MAP 0 //
#define kVOICE_6_TRANSPOSE_POST_NOTE_MAP 0 //

/* Wrapping Note Ranges */

#define kVOICE_1_LOWEST_NOTE 50 //Midi note number
#define kVOICE_2_LOWEST_NOTE 40
#define kVOICE_3_LOWEST_NOTE 50
#define kVOICE_4_LOWEST_NOTE 60
#define kVOICE_5_LOWEST_NOTE 60
#define kVOICE_6_LOWEST_NOTE 60

#define kVOICE_1_OCTAVE_RANGE 1 //Non-zero octave range (10 is 120 notes)
#define kVOICE_2_OCTAVE_RANGE 2
#define kVOICE_3_OCTAVE_RANGE 2
#define kVOICE_4_OCTAVE_RANGE 2
#define kVOICE_5_OCTAVE_RANGE 3
#define kVOICE_6_OCTAVE_RANGE 5

/* Arpeggiator Rates per instrument */

//floating point subdivision of measure (1.0f = whole note, 16.0f = 1/16th note)
//Decimal place (even if .0) and 'f' are important
#define kVOICE_1_ARP_SUBDIVISION 1.0f
#define kVOICE_2_ARP_SUBDIVISION 96.0f
#define kVOICE_3_ARP_SUBDIVISION 48.0f
#define kVOICE_4_ARP_SUBDIVISION 96.0f
#define kVOICE_5_ARP_SUBDIVISION 0.33f
#define kVOICE_6_ARP_SUBDIVISION 0.25f


/* Default smoothing, higher is slower (must be less than 1.0) */
#define kDEFAULT_SMOOTHING_RATE 0.99999

/* VOICE EFFECTS */

#define kVOICE_2_DELAY_DRY 0.0f
#define kVOICE_2_DELAY_WET 1.0f

//Delay time in 16th notes (integer values)
#define kVOICE_2_DELAY_TIME_STEP_1 2
#define kVOICE_2_DELAY_TIME_STEP_2 1
#define kVOICE_2_DELAY_TIME_STEP_3 3

#define kVOICE_2_FEEDBACK1 0.7f
#define kVOICE_2_FEEDBACK2 0.3f
#define kVOICE_2_FEEDBACK3 0.8f

#define kVOICE_3_DELAY_DRY 1.0f
#define kVOICE_3_DELAY_WET 0.0f

//Delay time in 16th notes (integer values)
#define kVOICE_3_DELAY_TIME_STEP_1 2
#define kVOICE_3_DELAY_TIME_STEP_2 1
#define kVOICE_3_DELAY_TIME_STEP_3 3

#define kVOICE_3_FEEDBACK1 0.0f
#define kVOICE_3_FEEDBACK2 0.0f
#define kVOICE_3_FEEDBACK3 0.0f

#define kVOICE_4_DELAY_DRY 1.0f
#define kVOICE_4_DELAY_WET 0.0f

//Delay time in 16th notes (integer values)
#define kVOICE_4_DELAY_TIME_STEP_1 2
#define kVOICE_4_DELAY_TIME_STEP_2 1
#define kVOICE_4_DELAY_TIME_STEP_3 3

#define kVOICE_4_FEEDBACK1 0.0f
#define kVOICE_4_FEEDBACK2 0.0f
#define kVOICE_4_FEEDBACK3 0.0f

#define kVOICE_5_DELAY_DRY 0.0f
#define kVOICE_5_DELAY_WET 1.0f

//Delay time in 16th notes (integer values)
#define kVOICE_5_DELAY_TIME_STEP_1 2
#define kVOICE_5_DELAY_TIME_STEP_2 1
#define kVOICE_5_DELAY_TIME_STEP_3 3

#define kVOICE_5_FEEDBACK1 0.7f
#define kVOICE_5_FEEDBACK2 0.3f
#define kVOICE_5_FEEDBACK3 0.8f

#define kVOICE_6_DELAY_DRY 0.5f
#define kVOICE_6_DELAY_WET 0.2f

//Delay time in 16th notes (integer values)
#define kVOICE_6_DELAY_TIME_STEP_1 2
#define kVOICE_6_DELAY_TIME_STEP_2 1
#define kVOICE_6_DELAY_TIME_STEP_3 3

#define kVOICE_6_FEEDBACK1 0.7f
#define kVOICE_6_FEEDBACK2 0.3f
#define kVOICE_6_FEEDBACK3 0.8f

typedef struct {
    float voice_gain;
    int voice_lowest_note;
    int voice_note_range;
    int voice_tranpose_pre_note_map; //deprecated
    int voice_tranpose_post_note_map; //deprecated
    float voice_delay_dry;
    float voice_delay_wet;
    int   voice_delay_time_step_1;
    int   voice_delay_time_step_2;
    int   voice_delay_time_step_3;
    float voice_delay_feedback_1;
    float voice_delay_feedback_2;
    float voice_delay_feedback_3;
} DGVoiceState;

typedef struct {
    
    /* TUNING PARAMETERS */
    char *name;

    float master_tempo;

    //#define kMASTER_GAIN 35.0 //Floating Point number greater than 0.0 (1.0 is unity) //4.0
    float master_gain;
    
    //#define kMASTER_REVERB_DRY 0.2f
    float master_reverb_dry;
    
    //#define kMASTER_REVERB_WET 2.0f
    float master_reverb_wet;
    
    //#define kBUS_A_RETURN_GAIN 0.2 //Floating Point number greater than 0.0 (1.0 is unity)
    float bus_a_return_gain;
    
    //#define kBUS_B_RETURN_GAIN 0.1 //Floating Point number greater than 0.0 (1.0 is unity)
    float bus_b_return_gain;
    
    DGVoiceState voices[4];
    
    BSRootKey rootKey;
    BSMusicalScale musicalScale;
    float tuningFrequency;
    
} DGEngineState;



#endif /* DGTuningParameters_hpp */
