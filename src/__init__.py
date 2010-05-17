from __future__ import absolute_import

from uinput import suinput

from .evtypes import EV_KEY
from .evtypes import EV_ABS
from .evtypes import EV_REL

from .bustypes import *
from .keycodes import *
from .abscodes import *
from .relcodes import *

class DeviceError(Exception):
    pass

class Device(object):
    def __init__(self, name="python-uinput",
                 bustype=BUS_VIRTUAL, vendor=0, product=0, version=3,
                 ff_effects_max=0):
        self._uinput_fd = None
        self._capability_catalogue = {}
        self._name = name
        self._bustype = bustype
        self._vendor = vendor
        self._product = product
        self._version = version
        self._ff_effects_max = ff_effects_max
        self._absmin = [0] * (ABS_CNT)
        self._absmax = [0] * (ABS_CNT)
        self._absfuzz = [0] * (ABS_CNT)
        self._absflat = [0] * (ABS_CNT)

    def set_abs_parameters(self, abs_code, abs_min=0, abs_max=0, abs_fuzz=0,
                           abs_flat=0):
        self._absmin[abs_code] = abs_min
        self._absmax[abs_code] = abs_max
        self._absfuzz[abs_code] = abs_fuzz
        self._absflat[abs_code] = abs_flat

    def get_abs_parameters(self, abs_code):
        return (self._absmin[abs_code], self._absmax[abs_code],
                self._absfuzz[abs_code], self._absflat[abs_code])

    def send(self, ev_type, ev_code, ev_value):
        if self._uinput_fd is None:
            self.activate()
        try:
            capability_set = self._capability_catalogue[ev_type]
            if ev_code not in capability_set:
                raise KeyError()
        except KeyError:
            raise DeviceError("Device is not capable of handling event.",
                              ev_type, ev_code)
        suinput.uinput_write(self._uinput_fd, ev_type, ev_code, ev_value)

    def activate(self):
        uinput_fd = suinput.uinput_open()
        try:
            for ev_type, capabilities in self._capability_catalogue.items():
                suinput.uinput_set_capabilities(uinput_fd, ev_type,
                                                capabilities)
            suinput.uinput_create(uinput_fd, self._name,
                                  self._bustype, self._vendor,
                                  self._product, self._version,
                                  self._ff_effects_max, self._absmin,
                                  self._absmax, self._absfuzz,
                                  self._absflat)
        except Exception:
            suinput.uinput_destroy(uinput_fd)
            return
        self._uinput_fd = uinput_fd

    def is_active(self):
        return self._uinput_fd is not None

    def syn(self):
        suinput.uinput_syn(self._uinput_fd)

    def add_capability(self, ev_type, ev_code):
        if self._uinput_fd is not None:
            raise DeviceError("Device is already active.")
        capabilities = self._capability_catalogue.setdefault(ev_type, set())
        capabilities.add(ev_code)

    def remove_capability(self, ev_type, ev_code):
        if self._uinput_fd is not None:
            raise DeviceError("Device is already active.")
        capability_set = self._capability_catalogue[ev_type]
        capability_set.remove(ev_code)
        if len(capability_set) == 0:
            self._capability_catalogue.pop(ev_type)

    @property
    def capabilities(self):
        return dict(self._capability_catalogue)

    def __del__(self):
        if self._uinput_fd is not None:
            suinput.uinput_destroy(self._uinput_fd)

class Capabilities(object):
    """Abstract class representing a set of input capabilities.
    Descendants must define _EV_TYPE."""

    def __init__(self, device):
        self._device = device

    def __iter__(self):
        try:
            return iter(self.device.capabilities[self._EV_TYPE])
        except KeyError:
            return iter(())

    def _send_to_device(self, ev_code, ev_value, syn):
        self.device.send(self._EV_TYPE, ev_code, ev_value)
        if syn:
            self.device.syn()

    @property
    def device(self):
        return self._device

    def add(self, ev_code):
        self.device.add_capability(self._EV_TYPE, ev_code)

    def remove(self, ev_code):
        self.device.remove_capability(self._EV_TYPE, ev_code)

class KeyCapabilities(Capabilities):

    _EV_TYPE = EV_KEY

    def press(self, key_code, syn=True):
        self._send_to_device(key_code, 1, syn)

    def release(self, key_code, syn=True):
        self._send_to_device(key_code, 0, syn)

    def click(self, key_code):
        self.press(key_code)
        self.release(key_code)

class RelativeAxisCapabilities(Capabilities):

    _EV_TYPE = EV_REL

    def move_by(self, rel_code, rel_value, syn=True):
        self._send_to_device(rel_code, rel_value, syn)

class AbsoluteAxisCapabilities(Capabilities):

    _EV_TYPE = EV_ABS

    def add(self, abs_code, abs_min=0, abs_max=255, abs_fuzz=0, abs_flat=0):
        Capabilities.add(self, abs_code)
        self.device.set_abs_parameters(abs_code, abs_min, abs_max, abs_fuzz,
                                       abs_flat)

    def move_to(self, abs_code, abs_value, syn=True):
        self._send_to_device(abs_code, abs_value, syn)
