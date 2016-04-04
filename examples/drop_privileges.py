import grp
import os
import pwd

import uinput

def drop_privileges(uid_name='nobody', gid_name='nogroup'):
    # https://stackoverflow.com/questions/2699907/dropping-root-permissions-in-python

    if os.getuid() != 0:
        return

    running_uid = pwd.getpwnam(uid_name).pw_uid
    running_gid = grp.getgrnam(gid_name).gr_gid

    os.setgroups([])
    os.setgid(running_gid)
    os.setuid(running_uid)
    old_umask = os.umask(0o77)

def main():
    uinput_fd = uinput.fdopen()
    drop_privileges() # No need to be root beyond this line.

    events = (uinput.KEY_A,)
    with uinput.Device(events, fd=uinput_fd) as device:
        device.emit_click(uinput.KEY_A)

if __name__ == "__main__":
    main()
