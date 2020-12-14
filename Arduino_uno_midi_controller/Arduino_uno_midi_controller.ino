#include <MIDI.h>

// MIDI defines
#define MOMENTARY 0
#define LATCHING 1
#define MODE MOMENTARY
// PINS
#define NUM_KEYS 16
#define CRASH_PIN A0
#define CHOKE_PIN 2 // usable for interrupts on NANO
#define HIHAT_PEDAL_PIN 3
#define HIHAT_PIN A1
#define KICK_PIN 3
// SIGNALS
#define KICK_SIGNAL 35
#define CHOKE_SIGNAL 58
#define CRASH_SIGNAL 57
#define HIHAT_PEDAL_SIGNAL 65
#define HIHAT_CLOSED_SIGNAL 42
#define HIHAT_OPEN_SIGNAL 46
#define NOTE_ON 144
#define NOTE_OFF 128
// MIN & MAX VALUES
#define MIDI_MAX_VALUE 127
#define CRASH_MIN_VAL 320
#define CRASH_MAX_VAL 370
#define CRASH_DIVIDER 300
#define HIHAT_MIN_VAL 439
#define HIHAT_MAX_VAL 446
#define HIHAT_DIVIDER 250
#define DEBOUNCE_CNT 30

//************ VARIABLES ************//
unsigned int velocity = 100; //velocity of MIDI notes, must be between 0 and 127
unsigned int noteON = 144; //144 = 10010000 in binary, note on command
unsigned int noteOFF = 128; //128 = 10000000 in binary, note off command

unsigned int maxi = 0, mini = 1024;

int crashValue = 0;
unsigned int crashHitCalc = 0;
unsigned int hihatHitCalc = 0;

int chokeFlag = 0;
volatile int chokeState = 0;
unsigned int chokeCnt = 0;

volatile int kickState = 0;
volatile int kickFlag = 0;
volatile int kickTim = 0;


//************ ***** ************//
//************ SETUP ************//
//************ ***** ************//
void setup() {
  // Set MIDI baud rate:
  // 9600 to interface with Hairless midi software
  Serial.begin(115200);
  pinMode(CRASH_PIN, INPUT);
  pinMode(CHOKE_PIN, INPUT);
  pinMode(KICK_PIN, INPUT);
  pinMode(HIHAT_PEDAL_PIN, INPUT);
  pinMode(HIHAT_PIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(KICK_PIN), kickDrum, FALLING);
}

//************ **** ************//
//************ LOOP ************//
//************ **** ************//
void loop() {
  // CRASH CYMBAL WITH CHOKE 
  unsigned int crashHit = analogRead(CRASH_PIN);
  if(crashHit < 320 || crashHit > 370) {
    hitCrash(crashHit);
  }
  if(digitalRead(CHOKE_PIN) == 0) {
    chokeCrash(); //dorobic semafor
  }

  // HIHAT with PEDAL
  //unsigned int hihatHit = analogRead(HIHAT_PIN);
  //if(hihatHit < 439){// || hihatHit > 460) {  // level on 441-444, mini= 0, maxi = 850
  //  hitHihat(hihatHit);
  //}
  /*if(digitalRead(HIHAT_PEDAL_PIN) == 0) {
    hihatPedal(); //dorobic semafor
  }*/
  
  // KICK DRUM
  /*
  if(kickFlag == 1) {
    kickTim++;
    if(kickTim > 50 && digitalRead(KICK_PIN) == 1) {
      kickFlag = 0;
      delay(10);
    }
  }
  if((!digitalRead(KICK_PIN)) && (kickFlag == 0)) {
    kickDrum();
    //while(!digitalRead(KICK_PIN));
  }*/
}

//************ FUNCTIONS ************//
void hitHihat(unsigned int hihatHit) {
  unsigned int val = 0;
  val = analogRead(HIHAT_PIN);
  while(val < hihatHit) {
    hihatHit = val;
    val = analogRead(HIHAT_PIN);
  }
  hihatHitCalc = ((HIHAT_MIN_VAL - hihatHit)*MIDI_MAX_VALUE)/HIHAT_DIVIDER;
  if(hihatHitCalc > MIDI_MAX_VALUE) hihatHitCalc = MIDI_MAX_VALUE;
  if(digitalRead(HIHAT_PEDAL_PIN) == 0) MIDImessage(noteON, HIHAT_CLOSED_SIGNAL, hihatHitCalc);
  else MIDImessage(noteON, HIHAT_OPEN_SIGNAL, hihatHitCalc);
  debounce(HIHAT_PIN, HIHAT_MAX_VAL, HIHAT_MIN_VAL);
}

void hitCrash(unsigned int crashHit) {
  unsigned int val = 0, cnt = 0;
  crashHitCalc = ((450 - crashHit)*MIDI_MAX_VALUE)/CRASH_DIVIDER; //450/300 because its easier to get higher value from crash piezo, might be necessary to adapt to individual needs
  if(crashHitCalc > MIDI_MAX_VALUE) crashHitCalc = MIDI_MAX_VALUE;
  MIDImessage(noteON, CRASH_SIGNAL, crashHitCalc);
  debounce(CRASH_PIN, 342, 336);
}

void chokeCrash() {
  delay(2);
  if(digitalRead(CHOKE_PIN) == 1) return;
  MIDImessage(noteON, CHOKE_SIGNAL, 1); //crashHitCalc
  while(digitalRead(CHOKE_PIN) == 0);
  delay(6);
}

void kickDrum() {
  unsigned int cnt = 0;
  MIDImessage(noteON, KICK_SIGNAL, 100);
  while(cnt < 15) {
    if(digitalRead(KICK_PIN) == 0) { // wait until its under specific level
      cnt++;
    }
    delay(1);
  }
  kickFlag = 1;
}

void debounce(int pin, int max_val, int min_val) {
  unsigned int val = 0, cnt = 0;
  while(cnt < DEBOUNCE_CNT) {
    val = analogRead(pin);
    if((val < max_val) && (val > min_val)) { // wait until its under specific level
      cnt++;
    }
    delay(1);
  }
}

  //send MIDI message through USB port
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}
