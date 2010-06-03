#! /usr/bin/env python

import time

import uinput

def main():
    capabilities = {uinput.EV_REL: [uinput.REL_X, uinput.REL_Y],
                    uinput.EV_KEY: [uinput.BTN_LEFT, uinput.BTN_RIGHT],
                    }

    device = uinput.Device(name="python-uinput-mouse",
                           capabilities=capabilities)

    for i in range(20):
        device.emit(uinput.EV_REL, uinput.REL_X, 5, syn=False)
        device.emit(uinput.EV_REL, uinput.REL_Y, 5)
        time.sleep(0.01) # Just to show the motion.

if __name__ == "__main__":
    main()
