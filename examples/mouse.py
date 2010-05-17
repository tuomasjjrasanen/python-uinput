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

    device.activate()
    time.sleep(1)
    # This magic sleep needs to be taken under X because of the way
    # how X assigns handlers for new devices:

    # 1. Kernel creates an evdev device.

    # 2. A dbus signal "DeviceAdded" is sent.

    # 3. X's handler assigner catches the dbus event and assigns a
    # handler for the new device.

    # Now the "problem" is in the asynchronous nature of this whole
    # process. Kernel is totally happy once it has created the evdev
    # device, but X cannot assign a handler for the new device before
    # it receives the dbus signal. Without this delay, kernel would
    # generate events before a handler is assigned. Perhaps a better
    # way would be to listen to the dbus inside the library?

    for i in range(20):
        rel_axes.move_by(uinput.REL_X, 5, False)
        rel_axes.move_by(uinput.REL_Y, 5)
        time.sleep(0.01) # Just to show the pointer motion.

    btns.click(uinput.BTN_LEFT)

if __name__ == "__main__":
    main()
