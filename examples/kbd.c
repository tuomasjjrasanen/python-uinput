#include <stdio.h>
#include <err.h>
#include <string.h>
#include <unistd.h>

#include <suinput.h>

int main(void)
{
    int uinput_fd;
    int keys[] = {KEY_E, KEY_H, KEY_L, KEY_O};
    struct uinput_user_dev user_dev;

    memset(&user_dev, 0, sizeof(struct uinput_user_dev));
    strcpy(user_dev.name, "libsuinput-example-keyboard");

    uinput_fd = suinput_open();

    if (uinput_fd == -1)
        err(1, "suinput_open");

    /* Error handling is omitted to keep code as readible as possible. */

    for (int i = 0; i < 4; ++i) {
        suinput_enable_event(uinput_fd, EV_KEY, keys[i]);
    }

    suinput_create(uinput_fd, &user_dev);

    suinput_emit(uinput_fd, EV_KEY, KEY_H, 1); /* Press. */
    suinput_syn(uinput_fd); /* "Flushes" events written so far. */

    suinput_emit(uinput_fd, EV_KEY, KEY_H, 0); /* Release */
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_E, 1);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_E, 0);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_L, 1);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_L, 0);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_L, 1);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_L, 0);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_O, 1);
    suinput_syn(uinput_fd);

    suinput_emit(uinput_fd, EV_KEY, KEY_O, 0);
    suinput_syn(uinput_fd);

    suinput_destroy(uinput_fd);

    return 0;
}
