/*
  DAC Melody player

  Generates a series of tones from MIDI note values
  using the Uno R4 DAC and the AnalogWave Library.
   The melody is "Frere Jacques"

circuit:
     * audio amp (LM386 used for testing) input+ attached to A0
     * audio amp input- attached to ground
     * 4-8-ohm speaker attached to amp output+
     * Potentiometer connected to pin A5

  created 13 Feb 2017
  modified 3 Jul 2023
  by Tom Igoe

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/dac
*/
#include "analogWave.h"
analogWave wave(DAC);

#define NOTE_A4 69         // MIDI note value for middle A
#define FREQ_A4 440        // frequency for middle A

// the tonic, or first note of the key signature for the song:
int tonic = 65;
// the melody sequence. Note values are relative to the tonic:
int melody[] = {1, 3, 5, 1,
                1, 3, 5, 1,
                5, 6, 8, 5, 6, 8,
                8, 10, 8, 6, 5, 1,
                8, 10, 8, 6, 5, 1,
                1, -4, 1,
                1, -4, 1
               };
// the rhythm sequence. Values are 1/note, e.g. 4 = 1/4 note:
int rhythm[] = {4, 4, 4, 4,
                4, 4, 4, 4,
                4, 4, 2,
                4, 4, 2,
                8, 8, 8, 8, 4, 4,
                8, 8, 8, 8, 4, 4,
                4, 4, 2,
                4, 4, 2
               };
// which note of the melody to play:
int noteCounter = 0;

int bpm = 120;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

void setup() {
// start the sine wave generator:
  wave.sine(10);
}

void loop() {
  // current note is an element of the array:
  int currentNote = melody[noteCounter] + tonic;
  // play a note from the melody:
  // convert MIDI note number to frequency:
  float frequency =  FREQ_A4 * pow(2, ((currentNote - NOTE_A4) / 12.0));

  // all the notes in this are sixteenth notes,
  // which is 1/4 of a beat, so:
  float noteDuration = beatDuration * (4.0 / rhythm[noteCounter]);
  // turn the note on:
  wave.freq(frequency);
 // tone(speakerPin, frequency, noteDuration * 0.85);
  // keep it on for the appropriate duration:
  delay(noteDuration * 0.85);
  wave.stop();
  delay(noteDuration * 0.15);
  // turn the note off:
 // noTone(speakerPin);
  // increment the note number for next time through the loop:
  noteCounter++;
  // keep the note in the range from 0 - 32 using modulo:
  noteCounter = noteCounter % 32;

}
