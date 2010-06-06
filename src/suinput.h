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
#ifndef SUINPUT_H
#define SUINPUT_H
#include <stdint.h>

#include <linux/uinput.h>

int suinput_write_event(int uinput_fd, const struct input_event *event);

int suinput_write(int uinput_fd, uint16_t ev_type, uint16_t ev_code,
                  int32_t ev_value);

int suinput_syn(int uinput_fd);

const char *suinput_get_uinput_path(void);

int suinput_open(void);

int suinput_create(int uinput_fd, const struct uinput_user_dev *user_dev);

int suinput_destroy(int uinput_fd);

int suinput_set_capabilities(int uinput_fd, uint16_t ev_type,
                             const int *ev_codes, size_t ev_codes_len);

#endif /* SUINPUT_H */
