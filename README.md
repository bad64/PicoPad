[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/) [![Visual Studio Code](https://img.shields.io/badge/--007ACC?logo=visual%20studio%20code&logoColor=ffffff)](https://code.visualstudio.com/)

Written in:         ![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)  
Powered by:         ![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)  
Platforms:          ![Steam](https://img.shields.io/badge/steam-%23000000.svg?style=for-the-badge&logo=steam&logoColor=white) ![Switch](https://img.shields.io/badge/Switch-E60012?style=for-the-badge&logo=nintendo-switch&logoColor=white)

[![Awesome Badges](https://img.shields.io/badge/badges-awesome-green.svg)](https://github.com/Naereen/badges)

# Multi Purpose Box Firmware

Once upon a midnight dreary, I was lurking on Discord, weak and weary, whereupon the topic of analog input was brought up by yet another lost soul looking to play Smash with a lever. Cue an almost year-long quest to make it a reality.

The project rapidly spiraled to include things (way) beyond its initial purpose of handling analog levers, mainly due to my own urge to build new controllers, and ended up as yet another configurable box controller firmware.

## Modes

The following modes are available to select from:

* Box layouts (SOCD is l+r=n only for now)
    * B0XX layout (18 buttons)
    * Frame1 layout (20 buttons)
    * WASD-style layout (20 buttons)
* Analog lever
    * Presets available for the Sanwa JLM and the Ultimarc Ultrastik U360
* Nunchuk (or any other i2c input such as mice, trackballs, etc)
* FGC (always accessible by holding A upon plugin)

## SOCD modes (when they're done ™)

* LRN: Known as the "default" mode for most all-button controllers nowadays
    * This is legal everywhere
    * Left + Right = Neutral
    * Up + Down = Up
* 2IP: For "Second Input Priority", also known as "Last Input Wins"
    * Might not be legal everywhere, consult your local TO for advice
    * Left + Right = Whatever was pressed last
    * Up + Down = Up
* YOLO: Let the game/host system decide and make TOs everywhere very very angry
    * Also known as "SOCD Unclean"
    * Just plain illegal (even if modern games do their own cleaning nowadays)
    * Left + Right = 🤷
    * Up + Down = 🤷

## Status

* All-button:       ![](https://geps.dev/progress/90)
    * └ Issue with action buttons not sticking (presumably general)
* Analog:           ![](https://geps.dev/progress/90)
    * └ Analog code complete, needs testing
* I2C:              ![](https://geps.dev/progress/50)
    * └ No analog to digital yet
    * └ Needs testing
* SOCD modes:       ![](https://geps.dev/progress/00)
    * └ Not started yet
* Gamecube mode:    ![](https://geps.dev/progress/00)
    * └ Not started yet
* Documentation:    ![](https://geps.dev/progress/33)
    * └ Aside from this README there isn't much substance (unless you count the anti-nerf manifesto)

## What's next ?

* Documentation
    * End-user manual
    * Tinkerer's guide
* SOCD modes in general
* Gamecube mode
* Some sort of purpose for Mid/Light shields for Ult ?
* Flasher (will be a separate project)

## Mad props to equally mad lads

These people contributed in some way in kicking my ass through this project, or motivating me to do the kicking myself:

* [AltLabs](http://www.altlabcontrollers.com/) for inspiring this whole project really
* [Haystack](https://github.com/JonnyHaystack/HayBox) for explaining USB like I'm five at a time where I really needed it, and for having a working implementation of the Nunchuk
* [Hadoe](https://github.com/HTangl/Model-UD) for the Model-UD schematic
* [Myrzon](https://www.etsy.com/shop/MyrzArcade?ref=simple-shop-header-name&listing_id=816502018) for the [panel mounted Nunchuk port](https://www.etsy.com/listing/816502018/nunchuk-controller-pass-through-breakout)
* [BitBang](https://bitbanggaming.com/) for teaching me the basics of embedded software development and PCB design
* [Kayane](https://twitter.com/kayane) for inspiring me to keep modding fightsticks
* The Mad Goose Labs for continuously wowing me with their insane projects
* Golden for the JLM group buy that kickstarted the entire project