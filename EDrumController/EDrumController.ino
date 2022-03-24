#include "src/DrumController/DrumController.hpp"
#include "src/DrumElement/ContinuousDrumElement.hpp"
#include "src/Utils/Constants.hpp"
#include <MIDI.h>

//************ VARIABLES ************//

int crashValue = 0;
long crashHitCalc = 0;
unsigned int hihatHitCalc = 0;
int diff = 0;

int chokeFlag = 0;
volatile int chokeState = 0;
unsigned int chokeCnt = 0;

volatile int kickState = 0;
volatile int kickFlag = 0;
volatile int kickTim = 0;

ContinuousDrumElement crashCymbal(CRASH_MID_VAL, CRASH_SIGNAL);
ContinuousDrumElement kickDrum(KICK_IDLE_VALUE, CRASH_SIGNAL);

DrumController controller;

//************ ***** ************//
//************ SETUP ************//
//************ ***** ************//
void setup()
{
    // Set MIDI baud rate:
    // 115200 to interface with Hairless midi software
    Serial.begin(115200);
    pinMode(CRASH_PIN, INPUT);
    pinMode(CHOKE_PIN, INPUT);
    // pinMode(KICK_PIN, INPUT); - TODO kick pin is the same as hihat pedal pin?
    pinMode(HIHAT_PEDAL_PIN, INPUT);
    pinMode(HIHAT_PIN, INPUT);
    controller.init();
}

//************ **** ************//
//************ LOOP ************//
//************ **** ************//
void loop()
{
    // CRASH CYMBAL
    uint16_t crashHit = analogRead(CRASH_PIN);
    crashCymbal.updateState(crashHit);

    // kick
    uint16_t kickHit = analogRead(CRASH_PIN);
    crashCymbal.updateState(kickHit);

    controller.cycle();
}

//************ FUNCTIONS ************//
void hitHihat(unsigned int hihatHit)
{
    unsigned int val = 0;
    val = analogRead(HIHAT_PIN);
    while(val < hihatHit)
    {
        hihatHit = val;
        val = analogRead(HIHAT_PIN);
    }
    hihatHitCalc = ((HIHAT_MIN_VAL - hihatHit) * MIDI_MAX_VALUE) / HIHAT_DIVIDER;
    if(hihatHitCalc > MIDI_MAX_VALUE)
        hihatHitCalc = MIDI_MAX_VALUE;
    if(digitalRead(HIHAT_PEDAL_PIN) == 0)
        MIDImessage(noteON, HIHAT_CLOSED_SIGNAL, hihatHitCalc);
    else
        MIDImessage(noteON, HIHAT_OPEN_SIGNAL, hihatHitCalc);
    debounce(HIHAT_PIN, HIHAT_MAX_VAL, HIHAT_MIN_VAL);
}

void hitCrash(unsigned int crashHit)
{
    unsigned int val = 0, cnt = 0;
    if(crashHit < CRASH_MID_VAL)
    {
        val = analogRead(CRASH_PIN);
        while(val < crashHit)
        {
            crashHit = val;
            val = analogRead(CRASH_PIN);
        }
    }
    else if(crashHit > CRASH_MID_VAL)
    {
        val = analogRead(CRASH_PIN);
        while(val > crashHit)
        {
            crashHit = val;
            val = analogRead(CRASH_PIN);
        }
    }
    diff = CRASH_MID_VAL - crashHit;
    diff = abs(diff);
    crashHitCalc = (diff * CRASH_COEF); //adjust CRASH_DIVIDER properly to the needs
    if(crashHitCalc > MIDI_MAX_VALUE)
        crashHitCalc = MIDI_MAX_VALUE;
    MIDImessage(noteON, CRASH_SIGNAL, crashHitCalc);
    debounce(CRASH_PIN, CRASH_MID_VAL + 4, CRASH_MID_VAL - 4);
}

void chokeCrash()
{
    delay(3);
    if(digitalRead(CHOKE_PIN) == 1)
        return;
    MIDImessage(noteON, CHOKE_SIGNAL, 1); //crashHitCalc
    while(digitalRead(CHOKE_PIN) == 0)
        ;
    delay(6);
}

void kickDrum()
{
    unsigned int cnt = 0;
    MIDImessage(noteON, KICK_MIDI_SIGNAL, 100);
    while(cnt < 15)
    {
        if(digitalRead(KICK_PIN) == 0)
        { // wait until its under specific level
            cnt++;
        }
        delay(1);
    }
    kickFlag = 1;
}

void debounce(int pin, int max_val, int min_val)
{
    unsigned int val = 0, cnt = 0;
    while(cnt < debounce_cnt)
    {
        val = analogRead(pin);
        if((val < max_val) && (val > min_val))
        { // wait until its under specific level
            cnt++;
        }
        delay(1);
    }
}

//send MIDI message through USB port
void MIDImessage(int command, int MIDInote, int MIDIvelocity)
{
    Serial.write(command); //send note on or note off command
    Serial.write(MIDInote); //send pitch data
    Serial.write(MIDIvelocity); //send velocity data
}
