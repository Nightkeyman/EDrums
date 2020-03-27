#include <MIDI.h>

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE MOMENTARY

// set to however many you're working with here
#define numKeys 16
#define crashPin A0
#define chokePin 2
#define kickPin 3

// signals
#define chokeSignal 56
#define crashSignal 57

unsigned int velocity = 100; //velocity of MIDI notes, must be between 0 and 127
unsigned int noteON = 144; //144 = 10010000 in binary, note on command
unsigned int noteOFF = 128; //128 = 10000000 in binary, note off command

unsigned int maxi = 0, mini = 1024;
unsigned int chokeCnt = 0;

int crashValue = 0;
volatile int chokeState = 0;
volatile int kickState = 0;
int chokeFlag = 0;
int kickFlag = 0;
unsigned int crashHitCalc = 0;

void setup() {
  // Set MIDI baud rate:
  // 9600 to interface with Hairless midi software
  Serial.begin(9600);
  pinMode(crashPin, INPUT);
  pinMode(chokePin, INPUT);
  pinMode(kickPin, INPUT);

  //attachInterrupt(digitalPinToInterrupt(chokePin), chokeCrash, FALLING);
}

void loop() {
  unsigned int crashHit = analogRead(crashPin);
  if(crashHit < 300 || crashHit > 440) {
    hitCrash(crashHit);
  }

  if(digitalRead(chokePin) == 0) {
    chokeCrash(); //dorobic semafor
  }
  
  if(!digitalRead(kickPin)) { //moze to jednak ADC ??
    kickDrum();
    delay(30);
    while(!digitalRead(kickPin));
  }
}

void hitCrash(unsigned int crashHit) {
  unsigned int val = 0, cnt = 0;
  crashHitCalc = ((450 - crashHit)*127)/300; //450/300 because its easier to get higher value from crash piezo, might be necessary to adapt to individual needs
  if(crashHitCalc > 127) crashHitCalc = 127;
  MIDImessage(noteON, crashSignal, crashHitCalc);
  
  while(cnt < 15) {
    val = analogRead(crashPin);
    if((val < 342) && (val > 336)) { // wait until its under specific level
      cnt++;
    }
    delay(1);
    //if(val < crashHit) return; // check if there was a new hit
    //Serial.println(val);
  }
  //delay(20);
}

void chokeCrash() {
  delay(3);
  if(digitalRead(chokePin) == 1) return;
  MIDImessage(noteON, chokeSignal, crashHitCalc);
  while(digitalRead(chokePin) == 0);
  delay(5);
}

void kickDrum() {
  MIDImessage(noteON, 35, 127);
}

  //send MIDI message through USB port
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}


