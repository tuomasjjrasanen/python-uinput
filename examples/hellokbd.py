#! /usr/bin/env python

import uinput

def main():
    device = uinput.Device()
    device.capabilities = {
        uinput.EV_KEY: (uinput.KEY_E, uinput.KEY_H, uinput.KEY_L, uinput.KEY_O),
        }

    device.emit(uinput.EV_KEY, uinput.KEY_H, 1) # Press.
    device.emit(uinput.EV_KEY, uinput.KEY_H, 0) # Release.
    device.emit(uinput.EV_KEY, uinput.KEY_E, 1)
    device.emit(uinput.EV_KEY, uinput.KEY_E, 0)
    device.emit(uinput.EV_KEY, uinput.KEY_L, 1)
    device.emit(uinput.EV_KEY, uinput.KEY_L, 0)
    device.emit(uinput.EV_KEY, uinput.KEY_L, 1)
    device.emit(uinput.EV_KEY, uinput.KEY_L, 0)
    device.emit(uinput.EV_KEY, uinput.KEY_O, 1)
    device.emit(uinput.EV_KEY, uinput.KEY_O, 0)

if __name__ == "__main__":
    main()
