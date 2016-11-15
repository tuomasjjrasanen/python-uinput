#!/usr/bin/env python
'''
Virtual Joystick from Keyboard
Bharat Tak
September 2016
'''

import uinput, time
import pygame, sys, os
from pygame.locals import *

pygame.init()
BLACK = (0,0,0)
WIDTH = 335
HEIGHT = 406
windowSurface = pygame.display.set_mode((WIDTH, HEIGHT), 0, 32)
windowSurface.fill(BLACK)
pygame.display.set_caption('Virtual RC Joystick')

# Fill background
background = pygame.Surface(windowSurface.get_size())
background = background.convert()
background.fill((250, 250, 250))

# Load image
dir = os.path.dirname(__file__)
filename = os.path.join(dir, 'media/sticks.png')
img = pygame.image.load(filename)

windowSurface.blit(img,(0,0))
pygame.display.flip()

class stick_state(object):
    def __init__(self, name, stick, key_up, key_down, spring_back=True, incr_val=0.2):
        self.name = name                            # The name of the stick
        self.stick = stick                          # The stick on the joystick that this stick maps to 
        self.key_up = key_up                        # The key on the keyboard that maps to this stick increment
        self.key_down = key_down                    # The key on the keyboard that maps to this stick decrement
        self.spring_back = spring_back              # Does the stick spring back to center on release?
        self.incr_val = incr_val                    # The increment on keypress
        self.min_val = 0.0                          # Minimum stick value
        self.max_val = 255.0                        # Maximum stick value
        self.active_up = False                      # True if up key is held pressed
        self.active_down = False                    # True if down key is held pressed
        if self.spring_back:
            self.zero = 127.0
        else:
            self.zero = 0.0
        self.val = self.zero                        # Stick value at initialization at zero position
        self.emit_val = int(self.val)
        self.display_ready = False                  # Whether optional display params have been set
        self.display_height = 0                     # Height on the display screen 
        self.display_width = 0                      # Width on the display screen 
        self.display_hor = True                     # Whether the display bar is horizontal, else vertical
        self.display_bar_g = []
        self.display_bar_b = []

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
            if self.display_ready:
                self.display()

    def set_display(self, offset_height, offset_width, horizontal):
        self.display_height = offset_height
        self.display_width = offset_width
        self.display_hor = horizontal
        if horizontal:
            filename = os.path.join(dir, 'media/hg.png')
            self.display_bar_g = pygame.image.load(filename)
            filename = os.path.join(dir, 'media/hb.png')
            self.display_bar_b = pygame.image.load(filename)
        else:
            filename = os.path.join(dir, 'media/vg.png')
            self.display_bar_g = pygame.image.load(filename)
            filename = os.path.join(dir, 'media/vb.png')
            self.display_bar_b = pygame.image.load(filename)    
        self.display_ready = True

    def display(self):
        if not self.display_ready:
            pass
        else:
            # Fill the entire bar
            for i in range(256):
                if i <= self.emit_val:
                    # Fill green
                    if self.display_hor:
                        windowSurface.blit(self.display_bar_g,(self.display_width + i, self.display_height))
                    else:
                        windowSurface.blit(self.display_bar_g,(self.display_width, self.display_height - i))
                else:
                    # Fill grey
                    if self.display_hor:
                        windowSurface.blit(self.display_bar_b,(self.display_width + i, self.display_height))
                    else:
                        windowSurface.blit(self.display_bar_b,(self.display_width, self.display_height - i))
            # Render it
            pygame.display.flip()

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

class switch_state(object):
    def __init__(self, name, switch, key, spring_back=True):
        self.name = name                            # The name of the button
        self.switch = switch                        # The switch on the joystick that this switch maps to
        self.key = key                              # The key on the keyboard that maps to this switch
        self.spring_back = spring_back              # Does the switch switch back to of-statee on release?
        self.active = False                         # True if key is held pressed
        self.active_previous = False                # Holds the previous active value
        self.display_ready = False                  # Whether optional display params have been set
        self.display_height = 0                     # Height on the display screen
        self.display_width = 0                      # Width on the display screen
        self.display_hor = True                     # Whether the display bar is horizontal, else vertical
        self.display_bar_g = []
        self.display_bar_b = []

    def keypress(self):
        self.active_previous = self.active
        if self.spring_back:
            self.active = True                      # self.active toggles automatically on key release
        else:
            self.active = not self.active           # toggle self.active on keypress

    def release_switch(self):
        if not self.spring_back:
            pass
        else:
            self.active_previous = self.active
            self.active = False

    def emit(self, device):
        # emit effeciently
        if self.active_previous != self.active:
            device.emit(self.switch, int(self.active))
            self.active_previous = self.active
            if self.display_ready:
                self.display()

    def set_display(self, offset_height, offset_width, horizontal):
        self.display_height = offset_height
        self.display_width = offset_width
        self.display_hor = horizontal
        if horizontal:
            filename = os.path.join(dir, 'media/hg.png')
            self.display_bar_g = pygame.image.load(filename)
            filename = os.path.join(dir, 'media/hb.png')
            self.display_bar_b = pygame.image.load(filename)
        else:
            filename = os.path.join(dir, 'media/vg.png')
            self.display_bar_g = pygame.image.load(filename)
            filename = os.path.join(dir, 'media/vb.png')
            self.display_bar_b = pygame.image.load(filename)
        self.display_ready = True

    def display(self):
        if not self.display_ready:
            pass
        else:
            # Fill the entire bar
            for i in range(256):
                if i <= self.emit_val:
                    # Fill green
                    if self.display_hor:
                        windowSurface.blit(self.display_bar_g,(self.display_width + i, self.display_height))
                    else:
                        windowSurface.blit(self.display_bar_g,(self.display_width, self.display_height - i))
                else:
                    # Fill grey
                    if self.display_hor:
                        windowSurface.blit(self.display_bar_b,(self.display_width + i, self.display_height))
                    else:
                        windowSurface.blit(self.display_bar_b,(self.display_width, self.display_height - i))
            # Render it
            pygame.display.flip()

    def update_event(self, event, device):
        if event.type == KEYUP:
            if (event.key == self.key):
                self.release_switch()
                self.emit(device)
        elif event.type == KEYDOWN:
            if (event.key == self.key):
                self.keypress()
                self.emit(device)

def main():
    events = (
        uinput.BTN_TRIGGER,
        uinput.BTN_A,
        uinput.BTN_B,
        uinput.BTN_C,
        uinput.ABS_X + (0, 255, 0, 0),
        uinput.ABS_Y + (0, 255, 0, 0),
        uinput.ABS_THROTTLE + (0, 255, 0, 0),
        uinput.ABS_RUDDER + (0, 255, 0, 0),
        )

    sticks = []
    switches = []

    # create sticks
    roll_stick = stick_state('Roll', uinput.ABS_X, K_RIGHT, K_LEFT)
    roll_stick.set_display(21, 39, True)
    sticks.append(roll_stick)
    pitch_stick = stick_state('Pitch', uinput.ABS_Y, K_UP, K_DOWN)
    pitch_stick.set_display(328, 198, False)
    sticks.append(pitch_stick)
    thr_stick = stick_state('Throttle', uinput.ABS_THROTTLE, K_w, K_s, False)
    thr_stick.set_display(328, 95, False)
    sticks.append(thr_stick)
    rud_stick = stick_state('Yaw', uinput.ABS_RUDDER, K_d, K_a)
    rud_stick.set_display(360, 39, True)
    sticks.append(rud_stick)

    # create switches
    switch_1 = switch_state('1', uinput.BTN_TRIGGER, K_q)
    switches.append(switch_1)
    switch_2 = switch_state('2', uinput.BTN_A, K_1, False)
    switches.append(switch_2)
    switch_3 = switch_state('3', uinput.BTN_B, K_2, False)
    switches.append(switch_3)
    switch_4 = switch_state('4', uinput.BTN_C, K_3, False)
    switches.append(switch_4)

    with uinput.Device(events) as device:
        while True:
            # event handling loop
            for event in pygame.event.get():
                for stick in sticks:
                    stick.update_event(event)
                for switch in switches:
                    switch.update_event(event, device)
            for stick in sticks:
                    stick.update_stick(device)
            time.sleep(0.0005)
            #device.emit_click(uinput.BTN_JOYSTICK)

if __name__ == "__main__":
    main()
