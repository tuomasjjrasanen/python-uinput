#include <time.h>
#include <suinput.h>

int
main(void)
{
  struct input_id id = {
    BUS_BLUETOOTH, /* Bus type. */
    13,            /* Vendor id. */
    3,             /* Product id. */
    7              /* Version id. */
  };
  int i;
  struct timespec pointer_motion_delay = {
    0,       /* Seconds. */
    25000000 /* Nanoseconds. */
  };
  int uinput_fd = suinput_open("HelloInput", &id);

  suinput_press(uinput_fd, KEY_LEFTSHIFT);
  suinput_click(uinput_fd, KEY_H);
  suinput_release(uinput_fd, KEY_LEFTSHIFT);
  suinput_click(uinput_fd, KEY_E);
  suinput_click(uinput_fd, KEY_L);
  suinput_click(uinput_fd, KEY_L);
  suinput_click(uinput_fd, KEY_O);
  suinput_click(uinput_fd, KEY_SPACE);
  suinput_click(uinput_fd, KEY_W);
  suinput_click(uinput_fd, KEY_O);
  suinput_click(uinput_fd, KEY_R);
  suinput_click(uinput_fd, KEY_L);
  suinput_click(uinput_fd, KEY_D);

  /* Assume that SHIFT+1 -> ! */
  suinput_press(uinput_fd, KEY_LEFTSHIFT);
  suinput_click(uinput_fd, KEY_1);
  suinput_release(uinput_fd, KEY_LEFTSHIFT);
  
  for (i = 0; i < 50; ++i) {
    suinput_move_pointer(uinput_fd, i, 10);
    nanosleep(&pointer_motion_delay, NULL);
  }

  for (i = 0; i < 50; ++i) {
    suinput_move_pointer(uinput_fd, -10, -i);
    nanosleep(&pointer_motion_delay, NULL);
  }

  suinput_close(uinput_fd);

  return 0;
}
