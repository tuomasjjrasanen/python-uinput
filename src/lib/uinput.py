from __future__ import absolute_import

import cUinput

class Driver(object):

    def __init__(self):
        self._input_fd = cUinput.open()

    def move_pointer(self, x, y):
        cUinput.move_pointer(self._input_fd, x, y)

    def press(self, key):
        cUinput.press(self._input_fd, key)

    def release(self, key):
        cUinput.release(self._input_fd, key)

    def click(self, key):
        self.press(key)
        self.release(key)

    def __del__(self):
        cUinput.close(self._input_fd)
