import uinput, time
import pygame, sys
from pygame.locals import *

pygame.init()
BLACK = (0,0,0)
WIDTH = 720
HEIGHT = 480
windowSurface = pygame.display.set_mode((WIDTH, HEIGHT), 0, 32)
windowSurface.fill(BLACK)


class stick_state(object):
    def __init__(self, stick, key_up, key_down, spring_back=True, incr_val=0.2):
        self.stick = stick                          # The stick on the joystick that this stick maps to 
        self.key_up = key_up                        # The key on the keyboard that maps to this stick increment
        self.key_down = key_down                    # The key on the keyboard that maps to this stick decrement
        self.spring_back = spring_back              # Does the stick spring back to center on release?
        self.incr_val = incr_val                    # The increment on keypress
        self.min_val = 0.0                          # Minimum stick value
        self.max_val = 255.0                        # Maximum stick value
        self.hold_time = 0                          # Last time(scaled) since the key is held
        self.rel_time = 0                           # Last time since the key has been released
        self.active_up = False                      # True if up key is held pressed
        self.active_down = False                    # True if down key is held pressed
        if self.spring_back:
            self.zero = 127.0
        else:
            self.zero = 0.0
        self.val = self.zero                         # Stick value at initialization at zero position
        self.emit_val = int(self.val)

    def keypress_up(self):
        self.active_up = True
        if (self.val + self.incr_val) <= self.max_val:
            self.val = self.val + self.incr_val
        else:
            # Saturated
            self.val = self.max_val

    def keypress_down(self):
        self.active_down = True
        if (self.val - self.incr_val) >= self.min_val:
            self.val = self.val - self.incr_val
        else:
            # Saturated
            self.val = self.min_val

    def incr_hold_time(self):
        self.hold_time = self.hold_time + 1

    def incr_rel_time(self):
        self.rel_time = self.rel_time + 1

    def release_stick(self):
        if not self.spring_back:
            pass
        else:
            if self.val > self.zero:
                self.val = self.val - self.incr_val*0.2
            elif self.val < self.zero:
                self.val = self.val + self.incr_val*0.2
            else:
                self.val = self.zero

    def emit(self, device):
        # emit effeciently
        if abs(int(round(self.val)) - int(self.emit_val)) > 0.001:
            self.emit_val = int(round(self.val))
            device.emit(self.stick, int(self.emit_val), syn=False)

    def update_event(self, event):
        if event.type == KEYUP:
            if (event.key == self.key_up):
                self.active_up = False
            elif event.key == self.key_down:
                self.active_down = False
        elif event.type == KEYDOWN:
            if (event.key == self.key_up):
                self.active = True
                self.keypress_up()
            elif (event.key == self.key_down):
                self.active = True
                self.keypress_down()

    def update_stick(self, device):
        if self.active_up:
            self.keypress_up()
            self.emit(device)
        elif self.active_down:
            self.keypress_down()
            self.emit(device)
        else:
            self.release_stick()
            self.emit(device)

def main():
    events = (
        uinput.BTN_JOYSTICK,
        uinput.ABS_X + (0, 255, 0, 0),
        uinput.ABS_Y + (0, 255, 0, 0),
        uinput.ABS_THROTTLE + (0, 255, 0, 0),
        uinput.ABS_RUDDER + (0, 255, 0, 0),
        )

    sticks = []

    # create sticks
    roll_stick = stick_state(uinput.ABS_X, K_RIGHT, K_LEFT)
    sticks.append(roll_stick)
    pitch_stick = stick_state(uinput.ABS_Y, K_UP, K_DOWN)
    sticks.append(pitch_stick)
    thr_stick = stick_state(uinput.ABS_THROTTLE, K_w, K_s, False)
    sticks.append(thr_stick)
    rud_stick = stick_state(uinput.ABS_RUDDER, K_d, K_a)
    sticks.append(rud_stick)

    with uinput.Device(events) as device:
        while True:
            # event handling loop
            for event in pygame.event.get():
                for stick in sticks:
                    stick.update_event(event)
            for stick in sticks:
                    stick.update_stick(device)
            time.sleep(0.0005)

if __name__ == "__main__":
    main()
