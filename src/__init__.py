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
>>> driver.click(uinput.codes.BTN_LEFT)
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

import codes

__all__ = [
    "Driver",
    ]

class Driver(object):

    """Device driver for the Linux uinput-system.

    For the documentation of the constructor arguments, see the
    documentation of the corresponding properties.
    """

    def __init__(self, name="python-uinput", bustype=_suinput.BUS_VIRTUAL,
                 vendor=0, product=0, version=0):
        self._input_fd = _suinput.open(name, bustype, vendor, product, version)
        self._name = name
        self._bustype = bustype
        self._vendor = vendor
        self._product = product
        self._version = version

    @property
    def name(self):
        "Kernel name of the device."
        return self._name

    @property
    def bustype(self):
        """Bustype of the device.
        Must be one of the BUS_ -prefixed constant values.
        """
        return self._bustype

    @property
    def vendor(self):
        """Vendor number of the device."""
        return self._vendor

    @property
    def product(self):
        """Product number of the device."""
        return self._product

    @property
    def version(self):
        """Version number of the device."""
        return self._version

    def move_pointer(self, x, y):
        """Sends a relative pointer motion event to the event device.
        Values increase towards right-bottom.
        """
        _suinput.move_pointer(self._input_fd, x, y)

    def press(self, uinput_code):
        """Sends a press event to the event device. Event is repeated after
        a short delay until a release event is sent.

        `uinput_code` must be one of the constant values defined in
        uinput.codes -module.
        """
        _suinput.press(self._input_fd, uinput_code)

    def release(self, uinput_code):
        """Sends a release event to the event device.

        `uinput_code` must be one of the constant values defined in
        uinput.codes -module.
        """
        _suinput.release(self._input_fd, uinput_code)

    def click(self, uinput_code):
        """Sends a press and release events to the event device.

        `uinput_code` must be one of the constant values defined in
        uinput.codes -module.

        This method is provided as a convenience and has effectively the
        same result as calling press(uinput_fd) and release(uinput_fd)
        sequentially.
        """
        _suinput.click(self._input_fd, uinput_code)

    def __del__(self):
        _suinput.close(self._input_fd)
