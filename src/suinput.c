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

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/uinput.h>

#include "suinput.h"

char* UINPUT_FILEPATHV[] = {"/dev/uinput", "/dev/input/uinput"};
#define UINPUT_FILEPATHC (sizeof(UINPUT_FILEPATHV) / sizeof(char*))

static int suinput_write(int uinput_fd,
                             uint16_t type, uint16_t code, int32_t value)
{
  struct input_event event;
  memset(&event, 0, sizeof(event));
  gettimeofday(&event.time, 0); /* This should not be able to fail ever.. */
  event.type = type;
  event.code = code;
  event.value = value;
  if (write(uinput_fd, &event, sizeof(event)) != sizeof(event))
    return -1;
  return 0;
}

static int suinput_write_syn(int uinput_fd,
                                 uint16_t type, uint16_t code, int32_t value)
{
  if (suinput_write(uinput_fd, type, code, value))
    return -1;
  return suinput_write(uinput_fd, EV_SYN, SYN_REPORT, 0);
}

int suinput_open(const char* device_name, const struct input_id* id)
{
  int original_errno;
  int i;
  int uinput_fd;

  for (i = 0; i < UINPUT_FILEPATHC; ++i) {
    uinput_fd = open(UINPUT_FILEPATHV[i], O_WRONLY | O_NONBLOCK);
    if (uinput_fd != -1)
      break;
  }

  if (uinput_fd == -1)
    return -1;

  /* Set device to handle following types of events: */

  /* Key and button events */
  if (ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY) == -1)
    goto err;

  /* Key and button repetition events */
  if (ioctl(uinput_fd, UI_SET_EVBIT, EV_REP) == -1)
    goto err;

  /* Relative pointer motions */
  if (ioctl(uinput_fd, UI_SET_EVBIT, EV_REL) == -1)
    goto err;

  /* Synchronization events, this is probably set implicitely too. */
  if (ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN) == -1)
    goto err;


  /* Configure device to handle relative x and y axis. */
  if (ioctl(uinput_fd, UI_SET_RELBIT, REL_X) == -1)
    goto err;

  if (ioctl(uinput_fd, UI_SET_RELBIT, REL_Y) == -1)
    goto err;


  /* Configure device to handle all more or less standard keys,
     see linux/input.h. */
  for (i = 0; i < KEY_MAX; i++) {
    if (ioctl(uinput_fd, UI_SET_KEYBIT, i) == -1)
      goto err;
  }

  /* Configure device to handle left and right mouse button events. */
  if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT) == -1)
    goto err;

  if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT) == -1)
    goto err;


  /* Set device-specific information. */
  struct uinput_user_dev user_dev;
  memset(&user_dev, 0, sizeof(user_dev));

  strncpy(user_dev.name, device_name, UINPUT_MAX_NAME_SIZE);
  user_dev.id.bustype = id->bustype;  
  user_dev.id.vendor = id->vendor;
  user_dev.id.product = id->product;
  user_dev.id.version = id->version;  

  if (write(uinput_fd, &user_dev, sizeof(user_dev)) != sizeof(user_dev))
    goto err;


  /* If all configurations above has been succesful, create the event device. */
  if (ioctl(uinput_fd, UI_DEV_CREATE) == -1)
    goto err;

  return uinput_fd;

 err:
  /* At this point, errno is set. close() can also fail and reset errno,
     so we store the original one and restore it if necessary. */     
  original_errno = errno;
  if (!close(uinput_fd))
    uinput_fd = -1; /* close() succeeded, fd is not needed anymore. */
  else
    errno = original_errno; /* Failed to close, reset original errno. */
  return -1;
}

int suinput_close(int uinput_fd)
{
  if (ioctl(uinput_fd, UI_DEV_DESTROY) == -1) {
    if (!close(uinput_fd)) {
      /* Succesful fd-closing despite the previous error:
         fd is not needed anymore. */
      uinput_fd = -1;
    }
    return -1;
  }

  if (close(uinput_fd) == -1)
    return -1;

  /* Succesful closing, fd is not needed anymore. */
  uinput_fd = -1;
  return 0;
}

int suinput_move_pointer(int uinput_fd, int32_t x, int32_t y)
{
  if (suinput_write(uinput_fd, EV_REL, REL_X, x))
    return -1;
  return suinput_write_syn(uinput_fd, EV_REL, REL_Y, y);
}

int suinput_press(int uinput_fd, uint16_t code)
{
  return suinput_write_syn(uinput_fd, EV_KEY, code, 1);
}

int suinput_release(int uinput_fd, uint16_t code)
{
  return suinput_write_syn(uinput_fd, EV_KEY, code, 0);
}

int suinput_click(int uinput_fd, uint16_t code)
{
  if (suinput_press(uinput_fd, code))
    return -1;
  return suinput_release(uinput_fd, code);
}
