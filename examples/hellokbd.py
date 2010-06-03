#! /usr/bin/env python

import time

import uinput

def main():
    device = uinput.Device()

    keys = uinput.KeyCapabilities(device)
    keys.add(uinput.KEY_E)
    keys.add(uinput.KEY_H)
    keys.add(uinput.KEY_L)
    keys.add(uinput.KEY_O)

    keys.click(uinput.KEY_H)
    keys.click(uinput.KEY_E)
    keys.click(uinput.KEY_L)
    keys.click(uinput.KEY_L)
    keys.click(uinput.KEY_O)

if __name__ == "__main__":
    main()
