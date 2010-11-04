#! /usr/bin/env python

"""
A joystick example.

Note that probably nothing visible happens when this script is run,
because joystick events (ABS_*) are not handled as a mouse events by
default.
"""

import uinput

def main():
    capabilities = {uinput.EV_ABS: [uinput.ABS_X, uinput.ABS_Y],
                    uinput.EV_KEY: [uinput.BTN_JOYSTICK],
                    }

    abs_parameters = {uinput.ABS_X: (0, 255, 0, 0), # min, max, fuzz, flat
                      uinput.ABS_Y: (0, 255, 0, 0)}

    device = uinput.Device(name="python-uinput-joystick",
                           capabilities=capabilities,
                           abs_parameters=abs_parameters)

    for i in range(20):
        # syn=False for the first call to emit an "atomic" (1, 1)
        # event.
        device.emit(uinput.EV_ABS, uinput.ABS_X, 1, syn=False)
        device.emit(uinput.EV_ABS, uinput.ABS_Y, 1)

    device.emit(uinput.EV_KEY, uinput.BTN_JOYSTICK, 1) # Press.
    device.emit(uinput.EV_KEY, uinput.BTN_JOYSTICK, 0) # Release.

if __name__ == "__main__":
    main()
