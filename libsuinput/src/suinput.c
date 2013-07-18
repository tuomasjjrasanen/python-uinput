/*
  libsuinput - thin userspace library on top of Linux uinput kernel module
  Copyright (C) 2013 Tuomas Räsänen <tuomasjjrasanen@tjjr.fi>

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
#include <stdlib.h>

#include <linux/limits.h>

#include "libudev.h"

#include "suinput.h"

int suinput_write_event(int uinput_fd, const struct input_event *event_p)
{
        ssize_t bytes;

        bytes = write(uinput_fd, event_p, sizeof(struct input_event));
        if (bytes != sizeof(struct input_event)) {
                return -1;
        }
        return 0;
}

int suinput_emit(int uinput_fd, uint16_t ev_type, uint16_t ev_code,
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

int suinput_emit_click(const int uinput_fd, const uint16_t key_code)
{
        if (suinput_emit(uinput_fd, EV_KEY, key_code, 1) == -1)
                return -1;
        return suinput_emit(uinput_fd, EV_KEY, key_code, 0);
}

int suinput_emit_combo(const int uinput_fd, const uint16_t *const key_codes,
                       const size_t len)
{
        int retval = 0;
        size_t i;

        for (i = 0; i < len; ++i) {
                if (suinput_emit(uinput_fd, EV_KEY, key_codes[i], 1) == -1) {
                        retval = -1;
                        break; /* The combination or the device is
                                  somehow broken: there's no sense to
                                  press any of the rest of the
                                  keys. It's like pressing physical keys
                                  one by one and then discovering that
                                  one of the keys required for this
                                  combination is missing or broken. */
                }
        }

        /* Try to release every pressed key, no matter what. */
        while (i--) {
                if (suinput_emit(uinput_fd, EV_KEY, key_codes[i], 0) == -1) {
                        retval = -1;
                }
        }

        return retval;
}

int suinput_syn(int uinput_fd)
{
        return suinput_emit(uinput_fd, EV_SYN, SYN_REPORT, 0);
}

static char *suinput_get_uinput_path(void)
{
        struct udev *udev;
        struct udev_device *udev_dev;
        const char *devnode;
        char *retval = NULL;
        int orig_errno;

        if ((udev = udev_new()) == NULL) {
                return NULL;
        }

        udev_dev = udev_device_new_from_subsystem_sysname(udev, "misc",
                                                          "uinput");
        if (udev_dev == NULL) {
                goto out;
        }

        if ((devnode = udev_device_get_devnode(udev_dev)) == NULL) {
                goto out;
        }

        if ((retval = malloc(strlen(devnode) + 1)) == NULL) {
                goto out;
        }

        strcpy(retval, devnode);
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
        char *uinput_path;

        if ((uinput_path = suinput_get_uinput_path()) == NULL) {
                return -1;
        }

        uinput_fd = open(uinput_path, O_WRONLY | O_NONBLOCK);
        free(uinput_path);
        return uinput_fd;
}

int suinput_create(int uinput_fd, const struct uinput_user_dev *user_dev_p)
{
        ssize_t bytes;

        bytes = write(uinput_fd, user_dev_p, sizeof(struct uinput_user_dev));
        if (bytes != sizeof(struct uinput_user_dev)) {
                return -1;
        }

        if (ioctl(uinput_fd, UI_DEV_CREATE) == -1) {
                return -1;
        }

        return 0;
}

int suinput_destroy(int uinput_fd)
{
        if (ioctl(uinput_fd, UI_DEV_DESTROY) == -1) {
                int original_errno = errno;
                close(uinput_fd);
                errno = original_errno;
                return -1;
        }

        return close(uinput_fd);
}

int suinput_enable_event(int uinput_fd, uint16_t ev_type, uint16_t ev_code)
{
        unsigned long io;

        if (ioctl(uinput_fd, UI_SET_EVBIT, ev_type) == -1) {
                return -1;
        }

        switch (ev_type) {
        case EV_KEY:
                io = UI_SET_KEYBIT;
                break;
        case EV_REL:
                io = UI_SET_RELBIT;
                break;
        case EV_ABS:
                io = UI_SET_ABSBIT;
                break;
        case EV_MSC:
                io = UI_SET_MSCBIT;
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
                errno = EINVAL;
                return -1;
        }

        return ioctl(uinput_fd, io, ev_code);
}
