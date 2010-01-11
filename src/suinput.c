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
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/uinput.h>

#include "suinput.h"

struct suinput_driver {
	int uinput_fd;
	uint8_t code_bits[KEY_MAX / 8 + 1];
};

char *UINPUT_FILEPATHS[] = {
	"/dev/uinput",
	"/dev/input/uinput",
	"/dev/misc/uinput",
};
#define UINPUT_FILEPATHS_COUNT (sizeof(UINPUT_FILEPATHS) / sizeof(char *))

#define KEY_MIN 1

static inline int test_code_bit(const struct suinput_driver *driver,
				uint16_t code)
{
	return driver->code_bits[code / 8] & (1 << (code % 8));
}

static int suinput_write(int uinput_fd, uint16_t type, uint16_t code,
			 int32_t value)
{
	struct input_event event;
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, 0);
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

struct suinput_driver *suinput_open(const char *device_name,
				    const struct input_id *id)
{
	int original_errno = 0;
	int uinput_fd = -1;
	struct uinput_user_dev user_dev;
	int i;
	struct suinput_driver *driver;

	for (i = 0; i < UINPUT_FILEPATHS_COUNT; ++i) {
		uinput_fd = open(UINPUT_FILEPATHS[i], O_WRONLY | O_NONBLOCK);
		if (uinput_fd != -1)
			break;
	}

	if (uinput_fd == -1)
		return NULL;

	if (ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY) == -1)
		goto err;

	if (ioctl(uinput_fd, UI_SET_EVBIT, EV_REP) == -1)
		goto err;

	if (ioctl(uinput_fd, UI_SET_EVBIT, EV_REL) == -1)
		goto err;

	if (ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN) == -1)
		goto err;

	if (ioctl(uinput_fd, UI_SET_RELBIT, REL_X) == -1)
		goto err;

	if (ioctl(uinput_fd, UI_SET_RELBIT, REL_Y) == -1)
		goto err;

	for (i = KEY_MIN; i < KEY_MAX; i++) {
		if (ioctl(uinput_fd, UI_SET_KEYBIT, i) == -1)
			goto err;
	}

	memset(&user_dev, 0, sizeof(user_dev));

	if (device_name)
		strncpy(user_dev.name, device_name, sizeof(user_dev.name));
	else
		strncpy(user_dev.name, "suinput driver", sizeof(user_dev.name));

	if (id) {
		user_dev.id.bustype = id->bustype;
		user_dev.id.vendor = id->vendor;
		user_dev.id.product = id->product;
		user_dev.id.version = id->version;
	} else {
		user_dev.id.bustype = BUS_VIRTUAL;
	}

	if (write(uinput_fd, &user_dev, sizeof(user_dev)) != sizeof(user_dev))
		goto err;

	if (ioctl(uinput_fd, UI_DEV_CREATE) == -1)
		goto err;

	driver = (struct suinput_driver *) malloc(sizeof(struct suinput_driver));
	if (driver == NULL)
		goto err;

	memset(driver, 0, sizeof(struct suinput_driver));
	driver->uinput_fd = uinput_fd;

	return driver;
err:
	original_errno = errno;
	close(uinput_fd);
	errno = original_errno;
	return NULL;
}

int suinput_close(struct suinput_driver *driver)
{
	int retval;
	int original_errno;

	if (ioctl(driver->uinput_fd, UI_DEV_DESTROY) == -1) {
		original_errno = errno;
		close(driver->uinput_fd);
		errno = original_errno;
		return -1;
	}

	retval = close(driver->uinput_fd);
	free(driver);
	return retval;
}

int suinput_move_pointer(struct suinput_driver *driver, int32_t x, int32_t y)
{
	if (suinput_write(driver->uinput_fd, EV_REL, REL_X, x))
		return -1;
	return suinput_write_syn(driver->uinput_fd, EV_REL, REL_Y, y);
}

int suinput_press(struct suinput_driver *driver, uint16_t code)
{
	int retval = suinput_write_syn(driver->uinput_fd, EV_KEY, code, 1);
	if (retval == -1)
		return retval;
	driver->code_bits[code / 8] |= 1 << (code % 8);
	return retval;
}

int suinput_release(struct suinput_driver *driver, uint16_t code)
{
	int retval = suinput_write_syn(driver->uinput_fd, EV_KEY, code, 0);
	if (retval == -1)
		return retval;
	driver->code_bits[code / 8] &= ~(1 << (code % 8));
	return retval;
}

int suinput_click(struct suinput_driver *driver, uint16_t code)
{
	if (suinput_press(driver, code))
		return -1;
	return suinput_release(driver, code);
}

int suinput_press_release(struct suinput_driver *driver, int16_t code)
{
	if (code > 0)
		return suinput_press(driver, code);
	return suinput_release(driver, abs(code));
}

int suinput_toggle(struct suinput_driver *driver, uint16_t code)
{
	if (suinput_is_valid_code(code)) {
		errno = EINVAL;
		return -1;
	}
	if (test_code_bit(driver, code))
		return suinput_release(driver, code);
	return suinput_press(driver, code);	
}

int suinput_is_pressed(const struct suinput_driver *driver, uint16_t code)
{
	if (suinput_is_valid_code(code))
		return -1;
	return test_code_bit(driver, code);
}

inline int suinput_is_valid_code(uint16_t code)
{
	return (KEY_MIN > code) || (code >= KEY_MAX);
}
