#include <unistd.h>
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

  int uinput_fd = suinput_open("Hello world input device", &id);

  /* Need to wait until the input device is created. This is going to
     be removed as soon as I figure out the best way to listen to the input
     device creation events.*/
  sleep(1);

  suinput_click(uinput_fd, KEY_H);
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
  
  suinput_move_pointer(uinput_fd, 100, 100);

  suinput_close(uinput_fd);

  return 0;
}
