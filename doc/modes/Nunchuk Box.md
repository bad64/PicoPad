# The one I made because of a joke that went way too far

Easy enough: i2c device goes in, analog goes out. This is intended to basically add a Nunchuk port to any box-type device, but realistically anything you can talk to via i2c can be communicated with so long as you adjust the right knobs and levers in the code.

## I2C and U

This mode is rather specifically tailored to talking with an official Nintendo Nunchuk. A cursory glance at `<lib/i2c/i2c.c>` shows that I use a state machine to communicate with the Nunchuk; as a result all time-critical functions are tailored to it and overall not very flexible. If you want to implement your own driver, you should definitely use it as a template to make things easier on yourself. (But again you do not *need* a state machine to do i2c comms properly; I choose to do it because in this case it was super convenient both in terms of implementation, explaining, and debugging purposes)

The concept of the state machine can be easily repurposed to be an output rather than an input, e.g. an OLED display mounted somewhere under your top acrylic plate.

## Port knocking

Looking for panel mounted Nunchuk ports ? [Here !](https://www.etsy.com/listing/816502018/nunchuk-controller-pass-through-breakout)