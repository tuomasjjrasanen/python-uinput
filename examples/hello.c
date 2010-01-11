#include <time.h>
#include <suinput.h>

int main(void)
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
	struct suinput_driver *driver = suinput_open("HelloInput", &id);

	/* sleep(1); */

	suinput_press(driver, KEY_LEFTSHIFT);
	suinput_click(driver, KEY_H);
	suinput_release(driver, KEY_LEFTSHIFT);
	suinput_click(driver, KEY_E);
	suinput_click(driver, KEY_L);
	suinput_click(driver, KEY_L);
	suinput_click(driver, KEY_O);
	suinput_click(driver, KEY_SPACE);
	suinput_click(driver, KEY_W);
	suinput_click(driver, KEY_O);
	suinput_click(driver, KEY_R);
	suinput_click(driver, KEY_L);
	suinput_click(driver, KEY_D);

	/* Assume that SHIFT+1 -> ! */
	suinput_press(driver, KEY_LEFTSHIFT);
	suinput_click(driver, KEY_1);
	suinput_release(driver, KEY_LEFTSHIFT);
  
	for (i = 0; i < 50; ++i) {
		suinput_move_pointer(driver, i, 10);
		nanosleep(&pointer_motion_delay, NULL);
	}

	for (i = 0; i < 50; ++i) {
		suinput_move_pointer(driver, -10, -i);
		nanosleep(&pointer_motion_delay, NULL);
	}

	/* sleep(1); */

	suinput_close(driver);

	return 0;
}
