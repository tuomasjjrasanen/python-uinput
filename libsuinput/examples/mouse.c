#include <stdio.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <suinput.h>

int main(void)
{
        int uinput_fd;
        int btns[] = {BTN_LEFT, BTN_RIGHT, BTN_MIDDLE};
        int rel_axes[] = {REL_X, REL_Y, REL_WHEEL};
        struct uinput_user_dev user_dev;
        int i;

        memset(&user_dev, 0, sizeof(struct uinput_user_dev));
        strcpy(user_dev.name, "libsuinput-example-mouse");

        uinput_fd = suinput_open();

        if (uinput_fd == -1)
                err(1, "suinput_open");

        /* Error handling is omitted to keep code as readible as possible. */

        for (i = 0; i < 3; ++i) {
                suinput_enable_event(uinput_fd, EV_KEY, btns[i]);
        }

        for (i = 0; i < 3; ++i) {
                suinput_enable_event(uinput_fd, EV_REL, rel_axes[i]);
        }

        suinput_create(uinput_fd, &user_dev);

        /* Move pointer 20 * 5 units towards bottom-right. */
        for (i = 0; i < 20; ++i) {
                struct timespec sleeptime = {0, 50000000};
                suinput_emit(uinput_fd, EV_REL, REL_X, 5);
                suinput_emit(uinput_fd, EV_REL, REL_Y, 5);
                suinput_syn(uinput_fd);

                nanosleep(&sleeptime, NULL);
        }

        suinput_emit_click(uinput_fd, BTN_LEFT);

        suinput_destroy(uinput_fd);

        return 0;
}
