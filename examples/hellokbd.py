#! /usr/bin/env python
# -*- coding: utf-8 -*-

import time

import uinput

def main():
    device = uinput.Device()

    keys = uinput.KeyCapabilities(device)
    keys.add(uinput.KEY_E)
    keys.add(uinput.KEY_H)
    keys.add(uinput.KEY_L)
    keys.add(uinput.KEY_O)

    device.activate()
    time.sleep(1)

    keys.click(uinput.KEY_H)
    keys.click(uinput.KEY_E)
    keys.click(uinput.KEY_L)
    keys.click(uinput.KEY_L)
    keys.click(uinput.KEY_O)

if __name__ == "__main__":
    main()
