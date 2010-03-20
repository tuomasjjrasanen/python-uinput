#! /usr/bin/env python
# -*- coding: utf-8 -*-

import time

import uinput

def main():
    device = uinput.Device()

    keys = uinput.KeyCapabilities(device)
    keys.add(uinput.KEY_H)
    keys.add(uinput.KEY_E)
    keys.add(uinput.KEY_L)
    keys.add(uinput.KEY_O)
    keys.add(uinput.BTN_LEFT)
    keys.add(uinput.BTN_RIGHT)
    keys.add(uinput.BTN_MIDDLE)

    rel_axes = uinput.RelativeAxisCapabilities(device)
    rel_axes.add(uinput.REL_X)
    rel_axes.add(uinput.REL_Y)
    rel_axes.add(uinput.REL_WHEEL)

    device.activate()
    time.sleep(1)

    keys.click(uinput.KEY_H)
    keys.click(uinput.KEY_E)
    keys.click(uinput.KEY_L)
    keys.click(uinput.KEY_L)
    keys.click(uinput.KEY_O)

    for i in range(20):
        rel_axes.move_by(uinput.REL_X, 5, False)
        rel_axes.move_by(uinput.REL_Y, 5)
        time.sleep(0.01)

if __name__ == "__main__":
    main()
