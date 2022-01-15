#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// Count the number of active notes to put the led on when at least one note is on
int nbNotesOn;

// -----------------------------------------------------------------------------

// This function will be automatically called when a NoteOn is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// https://github.com/FortySevenEffects/arduino_midi_library/wiki/Using-Callbacks
// Send all notes all channel 1
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  // Special notes driving the expressions
  if(pitch < 36){
    byte outChannel=3+(pitch-24)/6;
    byte outExpr=(pitch-24)%6*20;
    MIDI.sendControlChange(11, outExpr, outChannel);
  }else{
    MIDI.sendNoteOn(pitch, velocity, 1);
  }
  if(nbNotesOn == 0)digitalWrite(LED_BUILTIN, HIGH);  
  nbNotesOn++;
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    if(pitch >= 36)MIDI.sendNoteOff(pitch, velocity, 1);
    nbNotesOn--;
    if(nbNotesOn == 0)digitalWrite(LED_BUILTIN, LOW);  
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
}

// Send all program changes on channel 1 to channel 16
void handleProgramChange(byte channel, byte number)
{
  byte out;
  if(channel != 1)return;
  switch(number){
    case 24: out=2; break;
    case 25: out=1; break;
    case 26: out=3; break;
    case 27: out=4; break;
    case 28: out=5; break;
    case 29: out=6; break;
    case 30: out=7; break;
    case 32: out=8; break;
    case 33: out=9; break;
    case 34: out=10; break;
    case 35: out=11; break;
    case 36: out=12; break;
    case 38: out=13; break;
    case 39: out=14; break;
    case 105: out=15; break;
    case 106: out=16; break;
    default: out=number; break;
  }
  MIDI.sendProgramChange(out, 16);
}

// -----------------------------------------------------------------------------

void setup()
{
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleProgramChange(handleProgramChange);

    // Initiate MIDI communications, listen to all channels but disable thru
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();

    // initialize digital pin LED_BUILTIN as an output.
    nbNotesOn=0;
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();
}
