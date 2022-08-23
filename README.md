[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/) [![Visual Studio Code](https://img.shields.io/badge/--007ACC?logo=visual%20studio%20code&logoColor=ffffff)](https://code.visualstudio.com/)

# An analog fightstick... for Smashing

Once upon a midnight dreary, I was lurking on Discord, weak and weary, whereupon the topic of analog input was brought up by yet another lost soul looking to play Smash with a lever. Cue an almost year-long quest to make it a reality.

## What it do

This is the firmware for my custom Final Smash board (repo coming SOON(tm) ! It will be open source) that aims to allow enterprising makers to create a rectangle with an analog lever. Two such levers are planned to be supported: the Sanwa JLM (done) and the Ultrastik U360 (TBD, requires firmware that I cannot legally redistribute; you'll have to mail Andy at Ultimarc).

The board supports being plugged into a Switch to play Ultimate, a PC to play through Slippi, and eventually a Gamecube for Melee (some day).

## How it do

Once you installed the Raspberry Pi Pico SDK and its Visual Studio Code integration, just opening up the project and compiling with CMake will produce the appropriate UF2 binary that you can then upload to the board (or anything that uses an RP2040 really).

## Alternate modes

Hold the following buttons at boot for alternate input modes:

* A -> FGC mode; removes C-Stick input instead giving a standard 8-button fighter configuration
* B -> Separate D-Pad mode; Treats the D-Pad as completely separate from the left analog
* Start -> Nunchuk mode; Enables plugging a Nunchuk into the i2c port for left stick input (currently not implemented)
* Select -> BOOTSEL mode; aka what you need to flash new firmware onto the RP2040 (not actually an input mode but worth noting anyway)

Split D-Pad isn't mutually exclusive with other modes, it's merely a modifier pertaining to how the board should handle hat inputs.

Below is the pin map for the various modes: ("=" means identical to previous column; "^" means identical to previous row)

```
FGC MODE
    / BROOK 20pin        NORMAL MODE        NUNCHUK MODE        D-PAD MODE
-------------------------------------------------------------------------------
  Up/Down/Left/Right       Unused             Unused         Up/Down/Left/Right
        1P                 C-Left               =                   =
        2P                  C-Up                =                   =
        3P                 C-Right              =                   =
        4P                    Y                 =                   =
        1K                    B                 =                   =
        2K                    X                 =                   =
        3K                    ZR                =                   =
        4K                    R                 =                   =
    Not present          Not present          C-Down              Unused
    Not present          Not present            ZL                Unused
       Start                  +                 =            Whatever is used
      Select                  L                 =        by the other active mode
       Home                 Unused            Unused                ^
    Not present            Analog X           Unused                ^
    Not present            Analog Y           Unused                ^
    Not present               A                 =                   =
    Not present             C-Down              =                   =
```

Some notable things about this map:

* Yes, L as Select. Because of [the SWT ruleset](https://smashworldtour.com/wp-content/uploads/2022/03/SWT-2022-Rulebook.pdf), macros are disallowed for Melee. Including one for LRAStart. Therefore in order to not need three hands to reset, Select (also known as Minus, or "-" these days) goes away at the behest of L
* In FGC Mode, Start+Select produces a Home input
* FGC mode takes whatever directional input currently used

## What's next

* i2c Nunchuk mode
* Gamecube mode

## Mad props to equally mad lads

These people contributed in some way in kicking my ass through this project, or motivating me to do the kicking myself:

* [AltLabs](http://www.altlabcontrollers.com/) for inspiring this whole project really
* [Haystack](https://github.com/JonnyHaystack/HayBox) for explaining USB like I'm five at a time where I really needed it
* [Hadoe](https://github.com/HTangl/Model-UD) for the Model-UD schematic
* [BitBang](https://bitbanggaming.com/) for teaching me the basics of embedded software development and PCB design
* [Kayane](https://twitter.com/kayane) for inspiring me to keep modding fightsticks
* The Mad Goose Labs for continuously wowing me with their insane projects
* Golden for the JLM group buy that kickstarted the entire project