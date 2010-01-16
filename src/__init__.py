# -*- coding: utf-8 -*-
# uinput - Simple Python API to the Linux uinput-system
# Copyright (C) 2009 Tuomas Räsänen <tuos@codegrove.org>

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

"""Simple Python API to the Linux uinput-system

A high-level API to programmatically generate Linux input events.

Example usage:
>>> import uinput
>>> driver = uinput.Driver()
>>> driver.move_pointer(100, 100)
>>> driver.click(uinput.keycodes.BTN_LEFT)
"""

import _suinput
from _suinput import BUS_PCI
from _suinput import BUS_ISAPNP
from _suinput import BUS_USB
from _suinput import BUS_HIL
from _suinput import BUS_BLUETOOTH
from _suinput import BUS_VIRTUAL
from _suinput import BUS_ISA
from _suinput import BUS_I8042
from _suinput import BUS_XTKBD
from _suinput import BUS_RS232
from _suinput import BUS_GAMEPORT
from _suinput import BUS_PARPORT
from _suinput import BUS_AMIGA
from _suinput import BUS_ADB
from _suinput import BUS_I2C
from _suinput import BUS_HOST
from _suinput import BUS_GSC
from _suinput import BUS_ATARI

import keycodes

__all__ = [
    "Driver",
    ]

class Driver(object):
    """Device driver for the Linux uinput-system.

    keycodes are defined in uinput.keycodes -module.

    For the documentation of the constructor arguments, see the
    documentation of the corresponding properties.
    """

    def __init__(self, name="python-uinput", bustype=_suinput.BUS_VIRTUAL,
                 vendor=0, product=0, version=0):
        self._context = _suinput.open(name, bustype, vendor, product, version)
        self._name = name
        self._bustype = bustype
        self._vendor = vendor
        self._product = product
        self._version = version

    @property
    def name(self):
        "Name of the device."
        return self._name

    @property
    def bustype(self):
        """One of the BUS_ -prefixed constant values.
        """
        return self._bustype

    @property
    def vendor(self):
        """Arbitrary 16 bit unsigned integer vendor id."""
        return self._vendor

    @property
    def product(self):
        """Arbitrary 16 bit unsigned integer product id."""
        return self._product

    @property
    def version(self):
        """Arbitrary 16 bit unsigned integer version number."""
        return self._version

    def move_pointer(self, x, y):
        "Move pointer towards bottom-right."
        _suinput.move_pointer(self._context, x, y)

    def press(self, keycode):
        """Send a press event.
        Event is repeated after a short delay until a release event is sent."""
        _suinput.press(self._context, keycode)

    def release(self, keycode):
        "Send a release event."
        _suinput.release(self._context, keycode)

    def click(self, keycode):
        "Send a press and a release event."
        _suinput.click(self._context, keycode)

    def press_release(self, signed_keycode):
        """Send a press event if signed_keycode > 0, otherwise send
        a release event."""
        _suinput.press_release(self._context, signed_keycode)

    def toggle(self, keycode):
        "Press button if it is not pressed currently, release it otherwise."
        _suinput.toggle(self._context, keycode)

    def is_pressed(self, keycode):
        "Return True if button is pressed, otherwise return False."
        return _suinput.is_pressed(self._context, keycode)

    def __del__(self):
        _suinput.close(self._context)
