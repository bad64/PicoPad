[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/) [![Visual Studio Code](https://img.shields.io/badge/--007ACC?logo=visual%20studio%20code&logoColor=ffffff)](https://code.visualstudio.com/)

# Multi Purpose Box Firmware

Once upon a midnight dreary, I was lurking on Discord, weak and weary, whereupon the topic of analog input was brought up by yet another lost soul looking to play Smash with a lever. Cue an almost year-long quest to make it a reality.

The project rapidly spiraled to include things beyond its initial purpose of handling analog levers, mainly due to my own urge to build new controllers, and ended up as yet another configurable box controller firmware.

## Modes

The following modes are available to select from:

* Box layouts (SOCD is l+r=n only for now)
    * B0XX layout (18 buttons)
    * Frame1 layout (20 buttons)
    * WASD-style layout (20 buttons)
* Analog lever
    * Presets available for the Sanwa JLM and the Ultimarc Ultrastik U360
* Nunchuk (or any other i2c input such as mice, trackballs, etc)

## What's next ?

* Documentation
    * End-user manual
    * Mode-specific documentation
    * Tinkerer's guide
* 2IP SOCD type
* Gamecube mode
* Some sort of purpose for Mid/Light shields for Ult ?
* Flasher (will be a separate project)

## Mad props to equally mad lads

These people contributed in some way in kicking my ass through this project, or motivating me to do the kicking myself:

* [AltLabs](http://www.altlabcontrollers.com/) for inspiring this whole project really
* [Haystack](https://github.com/JonnyHaystack/HayBox) for explaining USB like I'm five at a time where I really needed it, and for having a working implementation of the Nunchuk
* [Hadoe](https://github.com/HTangl/Model-UD) for the Model-UD schematic
* [BitBang](https://bitbanggaming.com/) for teaching me the basics of embedded software development and PCB design
* [Kayane](https://twitter.com/kayane) for inspiring me to keep modding fightsticks
* The Mad Goose Labs for continuously wowing me with their insane projects
* Golden for the JLM group buy that kickstarted the entire project