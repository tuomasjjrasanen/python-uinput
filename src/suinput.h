/*
  suinput - Simple C-API to the Linux uinput-system.
  Copyright (C) 2009 Tuomas Räsänen <tuos@codegrove.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SUINPUT_H
#define SUINPUT_H
#include <stdint.h>

#include <linux/input.h>

/* Opaque object representing the input driver. */
struct suinput_driver;

/*
  Creates and opens a connection to the event device. Returns an opaque
  object representing the input driver on success. On error,
  NULL is returned, and errno is set appropriately.

  If device_name is NULL, "suinput driver" is used instead.
  If id is NULL, { BUS_VIRTUAL, 0, 0, 0 } is used instead.

   suinput_close() needs to be called to release the resources of the driver.
*/
struct suinput_driver *suinput_open(const char *device_name,
				    const struct input_id *id);

/* 
   Destroys and closes a connection to the event device. Returns 0 on success.
   On error, -1 is returned, and errno is set appropriately.

   This function has to be called exactly once for every opened driver.
*/
int suinput_close(struct suinput_driver *driver);

/*
  Sends a relative pointer motion event to the event device. Values increase
  towards right-bottom. Returns 0 on success. On error, -1 is returned, and
  errno is set appropriately.
*/
int suinput_move_pointer(struct suinput_driver *driver, int32_t x, int32_t y);

/*
  Sends a press event to the event device. Event is repeated after
  a short delay until a release event is sent. Returns 0 on success.
  On error, -1 is returned, and errno is set appropriately.
*/
int suinput_press(struct suinput_driver *driver, uint16_t code);

/*
  Sends a release event to the event device. Returns 0 on success.
  On error, -1 is returned, and errno is set appropriately.
*/
int suinput_release(struct suinput_driver *driver, uint16_t code);

/*
  Sends a press and release events to the event device. Returns 0 on
  success. On error, -1 is returned, and errno is set appropriately.

  This function is provided as a convenience and has effectively the
  same result as calling suinput_press() and suinput_release() sequentially.
*/
int suinput_click(struct suinput_driver *driver, uint16_t code);

/*
  Sends a press or a release event to the event device. The sign of
  code determines which type of event is sent. Positive code
  means press and negative code means release. Returns 0 on
  success. On error, -1 is returned, and errno is set appropriately.

  This function is provided as a convenience and has effectively the
  same result as calling suinput_press() when the value of code is
  positive and suinput_release() when negative.
*/
int suinput_press_release(struct suinput_driver *driver, int16_t code);

/*
  Sends a press or a release event to the event device. If a press event
  was sent previously, a release event is sent and vice versa.
  Returns 0 on success. On error, -1 is returned, and errno is set
  appropriately.
*/
int suinput_toggle(struct suinput_driver *driver, uint16_t code);

/*
  Returns 1 if a button or a key denoted by code is pressed
  and 0 if it is not.
*/
int suinput_is_pressed(const struct suinput_driver *driver, uint16_t code);

/*
  Returns 1 if code is valid, 0 otherwise.
  
  Valid codes are defined in linux/input.h prefixed by KEY_ or BTN_
  such that KEY_RESERVED < code < KEY_MAX.
*/
int suinput_is_valid_code(uint16_t code);

#endif /* SUINPUT_H */
