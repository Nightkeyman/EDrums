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
#define CRASH_MID_VAL 338
#define CRASH_COEF 0.4
#define HIHAT_MIN_VAL 437
#define HIHAT_MAX_VAL 448
#define HIHAT_MID_VAL 442
#define HIHAT_COEF 0.65
#define HIHAT_CLOSED 0
#define HIHAT_OPENED 1
#define DEBOUNCE_CNT 30

//************ VARIABLES ************//
unsigned int velocity = 100; //velocity of MIDI notes, must be between 0 and 127
unsigned int noteON = 144; //144 = 10010000 in binary, note on command
unsigned int noteOFF = 128; //128 = 10000000 in binary, note off command

unsigned int maxi = 0, mini = 1024;

int crashValue = 0;
long crashHitCalc = 0;
int diff = 0;
int chokeFlag = 0;
volatile int chokeState = 0;
unsigned int chokeCnt = 0;

unsigned int hihatHitCalc = 0;
int isHiHatOpened = HIHAT_OPENED;

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
  if(crashHit < CRASH_MIN_VAL || crashHit > CRASH_MAX_VAL) {
    hitCrash(crashHit);
  }
  if(digitalRead(CHOKE_PIN) == 0) {
    chokeCrash(); //dorobic semafor
  }

  // HIHAT with PEDAL
  unsigned int hihatHit = analogRead(HIHAT_PIN);
  if(hihatHit < HIHAT_MIN_VAL || hihatHit > HIHAT_MAX_VAL){
    hitHihat(hihatHit);
  }
  hiHatPedal();
  
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

//************ ********* ************//
//************ FUNCTIONS ************//
//************ ********* ************//
int hiHatPedal() {
  if(digitalRead(HIHAT_PEDAL_PIN) == HIHAT_OPENED && isHiHatOpened == HIHAT_CLOSED) {
    isHiHatOpened = HIHAT_OPENED;
  } else if(digitalRead(HIHAT_PEDAL_PIN) == HIHAT_CLOSED  && isHiHatOpened == HIHAT_OPENED){
    isHiHatOpened = HIHAT_CLOSED;
    MIDImessage(noteON, HIHAT_PEDAL_SIGNAL, MIDI_MAX_VALUE);
  }
}

void hitHihat(unsigned int hihatHit) {
  unsigned int val = 0;
  if(hihatHit < HIHAT_MID_VAL) {
    val = analogRead(HIHAT_PIN);
    while(val < hihatHit) {
      hihatHit = val;
      val = analogRead(HIHAT_PIN);
    }
  } else if(hihatHit > HIHAT_MID_VAL) {
    val = analogRead(HIHAT_PIN);
    while(val > hihatHit) {
      hihatHit = val;
      val = analogRead(HIHAT_PIN);
    }
  }
  diff = HIHAT_MID_VAL - hihatHit;
  diff = abs(diff);
  hihatHitCalc = (diff*HIHAT_COEF);
  if(hihatHitCalc > MIDI_MAX_VALUE) hihatHitCalc = MIDI_MAX_VALUE;
  if(isHiHatOpened == HIHAT_CLOSED) {
    MIDImessage(noteON, HIHAT_CLOSED_SIGNAL, hihatHitCalc);
  } else if(isHiHatOpened == HIHAT_OPENED) {
    MIDImessage(noteON, HIHAT_OPEN_SIGNAL, hihatHitCalc);
  }
  debounce(HIHAT_PIN, HIHAT_MAX_VAL, HIHAT_MIN_VAL);
}

void hitCrash(unsigned int crashHit) {
  unsigned int val = 0;
  if(crashHit < CRASH_MID_VAL) {
    val = analogRead(CRASH_PIN);
    while(val < crashHit) {
      crashHit = val;
      val = analogRead(CRASH_PIN);
    }
  } else if(crashHit > CRASH_MID_VAL) {
    val = analogRead(CRASH_PIN);
    while(val > crashHit) {
      crashHit = val;
      val = analogRead(CRASH_PIN);
    }
  }
  diff = CRASH_MID_VAL - crashHit;
  diff = abs(diff);
  crashHitCalc = (diff*CRASH_COEF); //adjust CRASH_DIVIDER properly to the needs
  if(crashHitCalc > MIDI_MAX_VALUE) crashHitCalc = MIDI_MAX_VALUE;
  MIDImessage(noteON, CRASH_SIGNAL, crashHitCalc);
  debounce(CRASH_PIN, CRASH_MID_VAL + 4, CRASH_MID_VAL - 4);
}

void chokeCrash() {
  delay(3);
  if(digitalRead(CHOKE_PIN) == 1) return;
  MIDImessage(noteON, CHOKE_SIGNAL, 1);
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
