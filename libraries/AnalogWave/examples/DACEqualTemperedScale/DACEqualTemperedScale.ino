/*
  Plays a tone in response to a potentiometer
  formula from https://newt.phys.unsw.edu.au/jw/notes.html
  and https://en.wikipedia.org/wiki/MIDI_tuning_standard:

  the MIDI protocol divides the notes of an equal-tempered scale into 
  128 possible note values. Middle A is MIDI note value 69. There is
  a formula for converting MIDI note numbers (0-127) to pitches. This sketch
  reduces that to the notes 21 - 108, which are the 88 keys found on a piano:

     frequency =  440 * 2^((noteNumber - 69) / 12.0)

  You can see this applied in the code below. 

  circuit:
     * audio amp (LM386 used for testing) input+ attached to A0
     * audio amp input- attached to ground
     * 4-8-ohm speaker attached to amp output+
     * Potentiometer connected to pin A5

   created 18 Dec 2018
   modified 3 Jul 2023
   by Tom Igoe

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/dac
*/

// include the AnalogWave library:
#include "analogWave.h"
analogWave wave(DAC);

// middle A is the reference frequency for an
// equal-tempered scale. Set its frequency and note value:
#define NOTE_A4 69         // MIDI note value for middle A
#define FREQ_A4 440        // frequency for middle A

const int speakerPin = A0;  // the pin number for the speaker
void setup() {
  Serial.begin(9600);
  wave.sine(10);
}
void loop() {
  // convert sensor reading to 21 - 108 range
  // which is the range of MIDI notes on an 88-key keyboard
  // (from A0 to C8):
  int sensorReading = analogRead(A5);
  int noteValue = map(sensorReading, 0, 1023, 21, 108);
  // then convert to frequency:
  float frequency =  FREQ_A4 * pow(2, ((noteValue - NOTE_A4) / 12.0));
  int freq = int(frequency);
  // turn the speaker on:
  wave.freq(freq);
  Serial.print("note value: "+ String(noteValue) + " freq: ");
  Serial.println(freq);
  delay(500);
}
