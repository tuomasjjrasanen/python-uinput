/*
  libsuinput - A set of uinput helper functions
  Copyright © 2010 Tuomas Räsänen (tuos) <tuos@codegrove.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#include <libudev.h>

#include "suinput.h"

int suinput_write_event(int uinput_fd, const struct input_event *event)
{
    size_t bytes;
    bytes = write(uinput_fd, event, sizeof(struct input_event));
    if (bytes != sizeof(struct input_event))
        return -1;
    return 0;
}

int suinput_write(int uinput_fd, uint16_t ev_type, uint16_t ev_code,
                  int32_t ev_value)
{
    struct input_event event;
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, 0);
    event.type = ev_type;
    event.code = ev_code;
    event.value = ev_value;
    return suinput_write_event(uinput_fd, &event);
}

int suinput_syn(int uinput_fd)
{
    return suinput_write(uinput_fd, EV_SYN, SYN_REPORT, 0);
}

const char *suinput_get_uinput_path(void)
{
    static char uinput_devnode[_POSIX_PATH_MAX + 1];
    struct udev *udev;
    struct udev_device *udev_dev;
    const char *devnode;
    const char *retval = NULL;
    int orig_errno;

    if ((udev = udev_new()) == NULL)
        return NULL;

    udev_dev = udev_device_new_from_subsystem_sysname(udev, "misc", "uinput");
    if (udev_dev == NULL)
        goto out;

    if ((devnode = udev_device_get_devnode(udev_dev)) == NULL)
        goto out;

    /* I'm on very defensive mood.. it's due the ignorance. :P */
    if (strlen(devnode) > _POSIX_PATH_MAX) {
        errno = ENAMETOOLONG;
        goto out;
    }

    strncpy(uinput_devnode, devnode, _POSIX_PATH_MAX);
    retval = uinput_devnode;
  out:
    orig_errno = errno;
    udev_device_unref(udev_dev);
    udev_unref(udev);
    errno = orig_errno;
    return retval;
}

int suinput_open(void)
{
    int uinput_fd;
    const char *uinput_devnode;

    if ((uinput_devnode = suinput_get_uinput_path()) == NULL)
        return -1;

    if ((uinput_fd = open(uinput_devnode, O_WRONLY | O_NONBLOCK)) == -1)
        return -1;

    return uinput_fd;
}

int suinput_create(int uinput_fd, const struct uinput_user_dev *user_dev)
{
    size_t bytes;

    bytes = write(uinput_fd, user_dev, sizeof(struct uinput_user_dev));
    if (bytes != sizeof(struct uinput_user_dev))
        return -1;

    if (ioctl(uinput_fd, UI_DEV_CREATE) == -1)
        return -1;

    /**
       This magic sleep needs to be taken under X because of the way
       how X assigns handlers for new devices. It goes somehow like
       this:

       1. Kernel creates an evdev device.

       2. A dbus signal "DeviceAdded" is sent.

       3. X's handler assigner catches the dbus event and assigns a
       handler for the new device.

       Now the "problem" is in the asynchronous nature of this whole
       process. Kernel is totally happy once it has created the evdev
       device, but X cannot assign a handler for the new device before
       it receives the dbus signal. Without this delay, kernel would
       generate events before a handler is assigned. Perhaps a better
       way would be to listen to the dbus?
    */
    if (getenv("DISPLAY"))
        sleep(1);

    return 0;
}

int suinput_destroy(int uinput_fd)
{
    int original_errno;

    if (ioctl(uinput_fd, UI_DEV_DESTROY) == -1) {
        original_errno = errno;
        close(uinput_fd);
        errno = original_errno;
        return -1;
    }

    return close(uinput_fd);
}

int suinput_set_capabilities(int uinput_fd, uint16_t ev_type,
                             const int *ev_codes, size_t ev_codes_len)
{
    int i;
    int io;

    if (ioctl(uinput_fd, UI_SET_EVBIT, ev_type) == -1)
        return -1;

    switch (ev_type) {
    case EV_REL:
        io = UI_SET_RELBIT;
        break;
    case EV_MSC:
        io = UI_SET_MSCBIT;
        break;
    case EV_KEY:
        io = UI_SET_KEYBIT;
        break;
    case EV_ABS:
        io = UI_SET_ABSBIT;
        break;
    case EV_SW:
        io = UI_SET_SWBIT;
        break;
    case EV_LED:
        io = UI_SET_LEDBIT;
        break;
    case EV_SND:
        io = UI_SET_SNDBIT;
        break;
    case EV_FF:
        io = UI_SET_FFBIT;
        break;
    default:
        return -2;
    }

    for (i = 0; i < ev_codes_len; ++i) {
        int ev_code = ev_codes[i];
        if (ioctl(uinput_fd, io, ev_code) == -1)
            return -1;
    }
    return 0;
}
