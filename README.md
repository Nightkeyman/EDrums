# EDrum
Electronic Drum controller

## How-to

- Connect arduino
- Turn-on hairless_midiserial (check BAUD rate! - it should match BAUD from Arduino) 
- set Arduino as input device (MIDI IN)
- Turn-on loopMIDI.exe
- Create virtual MIDI port in loopMIDI.exe
- in hairless_midiserial set loopMIDI as MIDI OUT
- set loopMIDI port in your DAW as input device (Reaper in my case)


## Hardware

- hardware setup: https://arduino.stackexchange.com/questions/32793/several-piezo-contact-mics-with-arduino?fbclid=IwAR0N-GsWEfaFlYdk2xDOvc0aYjVkZDs3TceU_XP_0vaBfH4HXn7DHbC4Z8U
