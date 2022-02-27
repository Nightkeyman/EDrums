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

## Build target (Arduino)

Using VS Code "Arduino" plugin by Microsoft.

## Build Unit Tests via CMake

Tools:
- googletest folder added as subfolder to EdrumController/test
- CMake
- Make

Commands for the GCC unit test build:
```console
cd ./Edrums (current folder)
cmake -S . -B ./build -G "Unix Makefiles"
cd ./build
make
cd ./EdrumController
./drumtests.exe
```

## Drum element algorithm

- lowpass filter applied
- check if filtered signal is above the idle value
- ... 
