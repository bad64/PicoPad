[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/) [![Visual Studio Code](https://img.shields.io/badge/--007ACC?logo=visual%20studio%20code&logoColor=ffffff)](https://code.visualstudio.com/)

# An analog fightstick... for Smashing

Once upon a midnight dreary, I was lurking on Discord, weak and weary, whereupon the topic of analog input was brought up by yet another lost soul looking to play Smash with a lever. Cue an almost year-long quest to make it a reality.

## What it do

This is the firmware for my custom Final Smash board (repo coming SOON(tm) ! It will be open source, trust me) that aims to allow enterprising makers to create a rectangle with an analog lever. Two such levers are planned: the Sanwa JLM (done) and the Ultrastik U360 (TBD, requires firmware that I cannot legally redistribute; you'll have to mail Andy at Ultimarc).

The board supports being plugged into a Switch to play Ultimate, a PC to play through Slippi, and eventually a Gamecube for Melee (some day).

## How it do

Once you installed the Raspberry Pi Pico SDK and its Visual Studio Code integration, just opening up the project and compiling with CMake will produce the appropriate UF2 binary that you can then upload to the board (or anything that uses an RP2040 really).

## Alternate modes

Hold the following buttons at boot for alternative input modes:

* A -> FGC mode; removes C-Stick input instead giving a standard 8-button fighter configuration
* B -> Separate D-Pad mode; Treats the D-Pad as completely separate from the left analog
* Start -> Nunchuk mode; Enables plugging a Nunchuk into the i2c port for left stick input (currently not implemented)
* Select -> BOOTSEL mode; aka what you need to flash new firmware onto the RP2040

Below is the pin map for the various modes: ("=" means identical)

```
FGC MODE
    / BROOK 20pin        NORMAL MODE        NUNCHUK MODE
--------------------------------------------------------
  Up/Down/Left/Right       Unused             Unused
        1P                 C-Left               =
        2P                  C-Up                =
        3P                 C-Right              =
        4P                    Y                 =
        1K                    B                 =
        2K                    X                 =
        3K                    ZR                =
        4K                    R                 =
      Unused                Unused            C-Down
      Unused                Unused              ZL
       Start                  +                 =
      Select                  L                 =
       Home                 Unused            Unused
      Unused               Analog X           Unused
      Unused               Analog Y           Unused
      Unused                  A                 =
      Unused                C-Down              =
```

## What's next

* i2c Nunchuk mode
* Gamecube comms

## Mad props to equally mad lads

These people contributed in some way in kicking my ass through this project, or motivating me to do the kicking myself:

* [AltLabs](http://www.altlabcontrollers.com/) for inspiring this whole project really
* [Haystack](https://github.com/JonnyHaystack/HayBox) for explaining USB like I'm five at a time where I really needed it
* [BitBang](https://bitbanggaming.com/) for teaching me the basics of embedded software development and PCB design
* [Kayane](https://twitter.com/kayane) for inspiring me to keep modding fightsticks
* The Mad Goose Labs for continuously wowing me with their insane projects
* Golden for the JLM group buy that kickstarted the entire project