import uinput

class Mouse(object):
    def __init__(self, *args, **kwargs):
        self.device = uinput.Device(*args, **kwargs)
        self.rel_axes = uinput.RelativeAxisCapabilities(self.device)
        self.rel_axes.add(uinput.REL_X)
        self.rel_axes.add(uinput.REL_Y)
        self.rel_axes.add(uinput.REL_WHEEL)
        self.keys = uinput.KeyCapabilities(self.device)
        self.keys.add(uinput.BTN_LEFT)
        self.keys.add(uinput.BTN_RIGHT)
        self.keys.add(uinput.BTN_MIDDLE)
        self.move_by = self.rel_axes.move_by
        self.press = self.keys.press
        self.release = self.keys.release
        self.click = self.keys.click

    def move(self, x, y):
        """Move pointer towards bottom right."""
        self.move_by(uinput.REL_X, x, False)
        self.move_by(uinput.REL_Y, y)

class Keyboard(object):
    def __init__(self, *args, **kwargs):
        self.device = uinput.Device(*args, **kwargs)
        self.keys = uinput.KeyCapabilities(self.device)
        self.keys.add(uinput.BTN_LEFT)
        self.keys.add(uinput.BTN_RIGHT)
        self.keys.add(uinput.BTN_MIDDLE)
        self.press = self.keys.press
        self.release = self.keys.release
        self.click = self.keys.click
