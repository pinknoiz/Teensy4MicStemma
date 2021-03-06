#include <Rhythm.h>
#include <Tap.h>

#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
//https://github.com/arkhipenko/TaskScheduler/wiki/API-Task

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "AudioSampleFrogloop16.h"

// ==== Debug and Test options ==================
#define _DEBUG_
//#define _TEST_

//===== Debugging macros ========================
#ifdef _DEBUG_
#define SerialD Serial
#define _PM(a) SerialD.print(millis()); SerialD.print(": "); SerialD.println(a)
#define _PP(a) SerialD.print(a)
#define _PL(a) SerialD.println(a)
#define _PX(a) SerialD.println(a, HEX)
#else
#define _PM(a)
#define _PP(a)
#define _PL(a)
#define _PX(a)
#endif


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

/*/// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=77,129
AudioMixer4              InputMixer;         //xy=250,141
AudioEffectChorus        chorus1;        //xy=455,243
AudioAnalyzeFFT1024      myFFT;          //xy=460,65
AudioSynthWaveform       waveform2;      //xy=461,322
AudioEffectChorus        chorus2;        //xy=466,383
AudioSynthWaveform       waveform1;      //xy=508,172
AudioMixer4              SourceMixer;         //xy=661.5555801391602,351.22221183776855
AudioFilterLadder        ladder1;        //xy=824.4444580078125,395.55554580688477
AudioMixer4              FXMixer;         //xy=966.7777671813965,342.3333435058594
AudioOutputI2S           i2s2;           //xy=1020.4444961547852,495.3333492279053
AudioConnection          patchCord1(i2s1, 0, InputMixer, 0);
AudioConnection          patchCord2(i2s1, 1, InputMixer, 1);
AudioConnection          patchCord3(InputMixer, myFFT);
AudioConnection          patchCord4(InputMixer, chorus1);
AudioConnection          patchCord5(InputMixer, chorus2);
AudioConnection          patchCord6(chorus1, 0, SourceMixer, 1);
AudioConnection          patchCord7(waveform2, 0, SourceMixer, 2);
AudioConnection          patchCord8(chorus2, 0, SourceMixer, 3);
AudioConnection          patchCord9(waveform1, 0, SourceMixer, 0);
AudioConnection          patchCord10(SourceMixer, 0, ladder1, 0);
AudioConnection          patchCord11(ladder1, 0, FXMixer, 0);
AudioConnection          patchCord12(FXMixer, 0, i2s2, 0);
AudioConnection          patchCord13(FXMixer, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=787,66
// GUItool: end automatically generated code
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
/*AudioPlayMemory          playMem1;       //xy=213,310
AudioEffectGranular      granular1;      //xy=492,438
AudioOutputI2S           i2s2;           //xy=1003,520
AudioConnection          patchCord1(playMem1, granular1);
AudioConnection          patchCord2(granular1, 0, i2s2, 0);
AudioConnection          patchCord3(granular1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=787,66
// GUItool: end automatically generated code
*/
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;       //xy=193,288
AudioEffectDelay         delay1;         //xy=376,475
AudioMixer4              mixer1;         //xy=384,311
AudioOutputI2S           i2s2;           //xy=1003,520
AudioConnection          patchCord1(playMem1, 0, mixer1, 0);
AudioConnection          patchCord2(delay1, 7, mixer1, 1);// feedback
AudioConnection          patchCord3(mixer1, delay1);
AudioConnection          patchCord4(mixer1, 0, i2s2, 0);
AudioConnection          patchCord5(mixer1, 0, i2s2, 1); 
AudioControlSGTL5000     sgtl5000_1;     //xy=787,66
// GUItool: end automatically generated code


#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
// Allocate the delay lines for left and right channels
short l_delayline[CHORUS_DELAY_LENGTH];
short r_delayline[CHORUS_DELAY_LENGTH];

Scheduler taskScheduler;

#define PERIOD1 20
void readControls();
Task tBlink1 (PERIOD1, TASK_FOREVER, &readControls, &taskScheduler, true);


#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

#define  DEFAULT_I2C_ADDR 0x30
#define  ANALOGIN   18
#define  NEOPIXELOUT 14

Adafruit_seesaw seesaw1;
Adafruit_seesaw seesaw2;
seesaw_NeoPixel pixels1 = seesaw_NeoPixel(4, NEOPIXELOUT, NEO_GRB + NEO_KHZ800);
seesaw_NeoPixel pixels2 = seesaw_NeoPixel(4, NEOPIXELOUT, NEO_GRB + NEO_KHZ800);

int baseFreq;
float deltaFreq;

void setup() {
  AudioMemory(120);
//  InputMixer.gain(0, 1.0);
//  InputMixer.gain(1, 1.0);
//  SourceMixer.gain(0, 0.25); //w
//  SourceMixer.gain(1, 0.50); //c
//  SourceMixer.gain(2, 0.25); //w
//  SourceMixer.gain(3, 0.50); //c
//  FXMixer.gain(0, 0.50); // 
//  FXMixer.gain(1, 0.50);
//  waveform1.begin(.5, 80, WAVEFORM_SAWTOOTH);
//  waveform2.begin(.5, 80.15, WAVEFORM_SAWTOOTH);

//  chorus1.begin(l_delayline,CHORUS_DELAY_LENGTH,2);
//  chorus1.begin(l_delayline,CHORUS_DELAY_LENGTH,2);
//  myFFT.windowFunction(AudioWindowHanning1024);

  mixer1.gain(0, .7);
  mixer1.gain(1, .25);
  delay1.delay(7, 110);
// Enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
//  sgtl5000_1.micGain(63);
//  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);

  if (!seesaw1.begin(DEFAULT_I2C_ADDR) || !seesaw2.begin(DEFAULT_I2C_ADDR+1)) {
    Serial.println(F("seesaws not found!"));
    while(1) delay(10);
  }
  Wire.begin();
}

void loop() {
  taskScheduler.execute();

  if (playMem1.isPlaying() == false) {// loop
//      uint16_t slide1_val = seesaw1.analogRead(ANALOGIN);
//
//    float msec = 10.0 + (slide1_val * 75.0);
//    granular1.beginFreeze(msec);
    playMem1.play(AudioSampleFrogloop16);
  }

  
/*  float n;
  int i;
  if (myFFT.available()) {
     each time new FFT data is available
     print it all to the Arduino Serial Monitor
    Serial.print("FFT: ");
    for (i=0; i<40; i++) {
      n = myFFT.read(i);
      if (n >= 0.01) {
        Serial.print(n);
        Serial.print(" ");
      } else {
        Serial.print("  -  "); // don't print "0.00"
      }
    }
    Serial.println();
  }
  */
}

inline void LEDOn() {
  digitalWrite( LED_BUILTIN, HIGH );
}

inline void LEDOff() {
  digitalWrite( LED_BUILTIN, LOW );
}

// === 1 =======================================
bool LED_state = false;
void readControls() {
  if ( LED_state ) {
    LEDOff();
    LED_state = false;
  }
  else {
    LEDOn();
    LED_state = true;
  }
  uint16_t slide1_val = seesaw1.analogRead(ANALOGIN);
  uint16_t slide2_val = seesaw2.analogRead(ANALOGIN);
  baseFreq = 40 + map(slide1_val, 0, 1023, 0, 120);
  deltaFreq = (float) map(slide2_val, 0, 1023, 1, 1000) / 1000.0f;
//  waveform1.frequency(baseFreq);
//  waveform2.frequency((float)baseFreq + deltaFreq);
//  float ratio;
//  ratio = powf(2.0, slide1_val * 2.0 - 1.0); // 0.5 to 2.0
//  _PP("ratio ");
//  _PL(ratio);
 
  //ratio = powf(2.0, knobA2 * 6.0 - 3.0); // 0.125 to 8.0 -- uncomment for far too much range!
//  granular1.setSpeed(.5);
  float feedback = map(slide1_val, 0, 1023,0, 900) / 1000.0f;

  mixer1.gain(1, feedback);
  int dtime = map(slide2_val, 0, 1023, 25, 330);
  delay1.delay(7, dtime);
    _PP("fdbk "); _PP(feedback);_PP(", time "); _PL(dtime);
}
