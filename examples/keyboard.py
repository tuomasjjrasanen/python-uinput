import uinput

def main():
    events = (
        uinput.KEY_E,
        uinput.KEY_H,
        uinput.KEY_L,
        uinput.KEY_O,
        )

    device = uinput.Device(events)

    device.emit_click(uinput.KEY_H)
    device.emit_click(uinput.KEY_E)
    device.emit_click(uinput.KEY_L)
    device.emit_click(uinput.KEY_L)
    device.emit_click(uinput.KEY_O)

if __name__ == "__main__":
    main()
