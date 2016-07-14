=======================================================
 Python-uinput NEWS -- history of user-visible changes
=======================================================

0.11.0
======

- New device method to close the device: ``Device.destroy()``

- ``Device()`` is now a context manager suitable to be used with
  ``with``-statement.

- Improved device initialization error handling.

- Updated README to include a recommendation to modprobe uinput kernel before
  usage.

- Launchpad is deprecated in favor of GitHub.

- New module function to open a file descriptor to uinput device: ``uinput.fdopen()``

- ``Device`` constructor accepts an open file descriptor to uinput
  device as a keyword argument. If not given, uinput file descriptor
  is opened via ``uinput.fdopen()``.

- Both ``README`` and ``NEWS`` are renamed and converted to
  reStructuredText markup.

- Input event codes included in ``uinput.ev`` module.

- Add example of how to drop privileges after device initialization
  (contributed by Goncalo Pinheira).

0.10.2
======

- Fix libudev1-compatibility issue.
- Fix mouse example.

0.10.1
======

- Fix Python3-compatibility issue.

0.10.0
======

- New device methods: ``Device.emit_click()`` and ``Device.emit_combo()``

- Keyboard example sleeps to give some time for X11 to assign a proper
  event handler

- Does not depend on libudev-dev anymore, because of newer libsuinput
  which re-distributes ``libudev.h``.

0.9
===

- Python3 compatibility
- Python2.6 compatibility

0.8
===

- device id fields can be given in the constructor: bustype, vendor,
  product and version

- an example of udev rule

0.7
===

- libsuinput is included as a subtree

0.6.1
=====

- ``README`` and ``setup.py`` updated

0.6
===

- Simplified API.

- Uses ``ctypes`` instead of extension modules.

- libsuinput is included as submodule in git repository

- source distributions include ``suinput.c``

0.5
===

- Documented uinput-module.

- A minimal joystick (abs-axis) example: ``examples/joystick.py``.

- Reasonable defaults for ``abs_parameters`` (0, 255, 0, 0).

0.4
===

- Updated project details, such as homepage, download url, etc.

0.3.3
=====

- Explicit dependence on libsuinput.

0.3.2
=====

- Mention Ubuntu-package in ``README``.

0.3.1
=====

- New license: GPLv3+
