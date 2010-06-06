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
from __future__ import absolute_import

from uinput import suinput

from .bustypes import *
from .capabilities import *

class Device(object):
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
        suinput.uinput_syn(self.__uinput_fd)

    @property
    def active(self):
        return self.__uinput_fd is not None

    def emit(self, ev_type, ev_code, ev_value, syn=True):
        if not self.active:
            self._activate()
        suinput.uinput_write(self.__uinput_fd, ev_type, ev_code, ev_value)
        if syn:
            self.syn()

    def __del__(self):
        if self.__uinput_fd is not None:
            suinput.uinput_destroy(self.__uinput_fd)
