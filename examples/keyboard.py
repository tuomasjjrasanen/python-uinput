import time

import uinput

def main():
    events = (
        uinput.KEY_E,
        uinput.KEY_H,
        uinput.KEY_L,
        uinput.KEY_O,
        )

    with uinput.Device(events) as device:
        time.sleep(1) # This is required here only for demonstration
                      # purposes. Without this, the underlying machinery might
                      # not have time to assign a proper handler for our device
                      # before the execution of this script reaches the end and
                      # the device is destroyed. At least this seems to be the
                      # case with X11 and its generic event device
                      # handlers. Without this magical sleep, "hello" might not
                      # get printed because this example exits before X11 gets
                      # its handlers ready for processing events from this
                      # device.
        device.emit_click(uinput.KEY_H)
        device.emit_click(uinput.KEY_E)
        device.emit_click(uinput.KEY_L)
        device.emit_click(uinput.KEY_L)
        device.emit_click(uinput.KEY_O)

if __name__ == "__main__":
    main()
