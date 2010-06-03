#! /usr/bin/env python

import time

import uinput

def main():
    device = uinput.Device()

    btns = uinput.KeyCapabilities(device)
    for btn in (uinput.BTN_LEFT, uinput.BTN_RIGHT, uinput.BTN_MIDDLE):
        btns.add(btn)

    rel_axes = uinput.RelativeAxisCapabilities(device)
    for rel_axis in (uinput.REL_X, uinput.REL_Y, uinput.REL_WHEEL):
        rel_axes.add(rel_axis)

    for i in range(20):
        rel_axes.move_by(uinput.REL_X, 5, False)
        rel_axes.move_by(uinput.REL_Y, 5)
        time.sleep(0.01) # Just to show the pointer motion.

    btns.click(uinput.BTN_LEFT)

if __name__ == "__main__":
    main()
