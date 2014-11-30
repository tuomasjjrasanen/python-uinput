import uinput

def main():
    events = (
        uinput.BTN_JOYSTICK,
        uinput.ABS_X + (0, 255, 0, 0),
        uinput.ABS_Y + (0, 255, 0, 0),
        )

    with uinput.Device(events) as device:
        for i in range(20):
            # syn=False to emit an "atomic" (5, 5) event.
            device.emit(uinput.ABS_X, 5, syn=False)
            device.emit(uinput.ABS_Y, 5)
        device.emit_click(uinput.BTN_JOYSTICK)

if __name__ == "__main__":
    main()
