# -*- coding: utf-8 -*-
# uinput - Uinput API for Python
# Copyright © 2010 Tuomas Räsänen (tuos) <tuos@codegrove.org>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""
Python Uinput API for creating Linux input device drivers.

Usage:
>>> capabilities = {uinput.EV_ABS: (uinput.ABS_X, uinput.ABS_Y)}
>>> abs_parameters = {uinput.ABS_X: (0, 255, 0, 0), # min, max, fuzz, flat
>>>                   uinput.ABX_Y: (0, 255, 0, 0)}
>>> driver = uinput.Device(name="my-device-driver",
                           capabilities=capabilities,
                           abs_parameters=abs_parameters)
>>> driver.emit(uinput.EV_ABS, uinput.ABS_X, 5, syn=False)
>>> driver.emit(uinput.EV_ABS, uinput.ABS_Y, 5)
"""

from __future__ import absolute_import

from uinput import suinput

from .bustypes import *
from .capabilities import *

class Device(object):

    """
    Create a Uinput device.

    `name`           - a string displayed in /proc/bus/input/devices

    `bustype`        - uinput.BUS_*

    `vendor`         - vendor id as an integer value

    `product`        - product id as an integer value

    `version`        - version id as an integer value

    `ff_effects_max` - 

    `capabilities`   - a dict describing capabilities of the device,
                       for example for EV_REL and EV_ABS capable device:
                       {uinput.EV_REL: (uinput.REL_X, uinput.REL_Y),
                        uinput.EV_ABS: (uinput.ABS_X, uinput.ABS_Y)}

    `abs_parameters` - a dict describing parameters of absolute axes,
                       for example:
                       {uinput.ABS_X: (0, 255, 0, 0), # min, max, fuzz, flat
                        uinput.ABS_Y: (0, 255, 0, 0)}
    """

    def __init__(self, name="python-uinput",
                 bustype=BUS_VIRTUAL, vendor=0, product=0, version=3,
                 ff_effects_max=0, capabilities={}, abs_parameters={}):
        self.__uinput_fd = None
        self.capabilities = capabilities
        self.name = name
        self.bustype = bustype
        self.vendor = vendor
        self.product = product
        self.version = version
        self.ff_effects_max = ff_effects_max
        self.abs_parameters = abs_parameters

    def _activate(self):
        fd = suinput.uinput_open()
        try:
            for ev_type, capabilities in self.capabilities.items():
                suinput.uinput_set_capabilities(fd, ev_type, set(capabilities))
            abs_min_vals = ABS_CNT * [0]
            abs_max_vals = ABS_CNT * [0]
            abs_fuzz_vals = ABS_CNT * [0]
            abs_flat_vals = ABS_CNT * [0]
            for abs_code, (abs_min, abs_max, abs_fuzz, abs_flat) in self.abs_parameters.items():
                abs_min_vals[abs_code] = abs_min
                abs_max_vals[abs_code] = abs_max
                abs_fuzz_vals[abs_code] = abs_fuzz
                abs_flat_vals[abs_code] = abs_flat

            suinput.uinput_create(fd,
                                  self.name,
                                  self.bustype,
                                  self.vendor,
                                  self.product,
                                  self.version,
                                  self.ff_effects_max,
                                  abs_min_vals,
                                  abs_max_vals,
                                  abs_fuzz_vals,
                                  abs_flat_vals)
        except Exception, e:
            suinput.uinput_destroy(fd)
            raise e
        self.__uinput_fd = fd

    def syn(self):
        """
        Fire all emitted events.  All emitted events will be placed in
        a certain kind of event queue. Queued events will be fired when this
        method is called.  This is makes it possible to emit "atomic"
        events. For example sending REL_X and REL_Y atomically requires
        to emit first event without syn and the second with syn::

          d.emit(uinput.EV_REL, uinput.REL_X, 1, syn=False)
          d.emit(uinput.EV_REL, uinput.REL_Y, 1)

        The call above appears as a single (+1, +1) event.
        
        """
        suinput.uinput_syn(self.__uinput_fd)

    @property
    def active(self):
        """
        Return boolean value indicating whether the device is
        activated. Once the device is activated, changing attributes does not
        have any effect on the driver behavior.
        """
        return self.__uinput_fd is not None

    def emit(self, ev_type, ev_code, ev_value, syn=True):
        """
        Emit event.
        If the device is not yet active, activate it first.

        `ev_type`  - uinput.EV_[TYPE], for example uinput.EV_ABS
        `ev_code`  - uinput.[TYPE]_[CODE], for example uinput.ABS_X
        `ev_value` - value of the event type:
           EV_KEY/EV_BTN: 1 (key-press) or 0 (key-release)
           EV_REL       : integer value of the relative change
           EV_ABS       : integer value in the range of min and max values as
                          defined in self.abs_parameters
        `syn`      - If True, Device.syn(self) will be called before return.
        """
        if not self.active:
            self._activate()
        suinput.uinput_write(self.__uinput_fd, ev_type, ev_code, ev_value)
        if syn:
            self.syn()

    def __del__(self):
        if self.__uinput_fd is not None:
            suinput.uinput_destroy(self.__uinput_fd)
