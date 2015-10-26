import uinput

def drop_privileges(uid_name='nobody', gid_name='nogroup'):
    #https://stackoverflow.com/questions/2699907/dropping-root-permissions-in-python
    import os, pwd, grp
    if os.getuid() != 0:
        return
    running_uid = pwd.getpwnam(uid_name).pw_uid
    running_gid = grp.getgrnam(gid_name).gr_gid
    os.setgroups([])
    os.setgid(running_gid)
    os.setuid(running_uid)
    old_umask = os.umask(077)


def main():
    uinput_fd= uinput.Device.create_uinput_fd()
    drop_privileges() #no need to be root beyond this line

    events = (uinput.KEY_A,)
    with uinput.Device(events, fd=uinput_fd) as device:
        device.emit_click(uinput.KEY_A)

if __name__ == "__main__":
    main()
