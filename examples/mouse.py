#! /usr/bin/env python
# -*- coding: utf-8 -*-

import time

import uinput

def main():
    device = uinput.Device()

    btns = uinput.KeyCapabilities(device)
    btns.add(uinput.BTN_LEFT)
    btns.add(uinput.BTN_RIGHT)
    btns.add(uinput.BTN_MIDDLE)

    rel_axes = uinput.RelativeAxisCapabilities(device)
    rel_axes.add(uinput.REL_X)
    rel_axes.add(uinput.REL_Y)
    rel_axes.add(uinput.REL_WHEEL)

    device.activate()
    time.sleep(1)

    for i in range(20):
        rel_axes.move_by(uinput.REL_X, 5, False)
        rel_axes.move_by(uinput.REL_Y, 5)
        time.sleep(0.01)

    btns.click(uinput.BTN_LEFT)

if __name__ == "__main__":
    main()
