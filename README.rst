===============
 Python-uinput
===============

Python-uinput is Python interface to Linux uinput kernel module which
allows attaching userspace device drivers into kernel. In practice,
Python-uinput makes it dead simple to create virtual joysticks,
keyboards and mice for generating arbitrary input events
programmatically.

:Homepage: http://tjjr.fi/sw/python-uinput/
:License: GPLv3+

Usage examples
==============

Generate keyboard clicks
------------------------

::

    import uinput

    with uinput.Device([uinput.KEY_E, uinput.KEY_H,
                        uinput.KEY_L, uinput.KEY_O]) as device:
        device.emit_click(uinput.KEY_H)
        device.emit_click(uinput.KEY_E)
        device.emit_click(uinput.KEY_L)
        device.emit_click(uinput.KEY_L)
        device.emit_click(uinput.KEY_O)

Move mouse cursor
-----------------

::

    import uinput

    with uinput.Device([uinput.REL_X, uinput.REL_Y,
                        uinput.BTN_LEFT, uinput.BTN_RIGHT]) as device:
        for i in range(20):
            device.emit(uinput.REL_X, 5)
            device.emit(uinput.REL_Y, 5)

Generate keyboard combinations
------------------------------

::

    import uinput

    with uinput.Device([uinput.KEY_LEFTALT, uinput.KEY_TAB]) as device:
        device.emit_combo([uinput.KEY_LEFTALT, uinput.KEY_TAB])

See ``examples/`` for other kind of example devices (keyboard, joystick).

Note that you must have the uinput kernel module loaded to use
python-uinput. To load the module, run::

    modprobe -i uinput

If you would like to have uinput to be loaded on every sytem boot, add
uinput to ``/etc/modules``.

How to install
==============

Just run the following commands::

    python setup.py build
    python setup.py install

How to report bugs
==================

The bug database is hosted in GitHub. If you have found a bug or have an
improvement idea you want to share, please report it at
<https://github.com/tuomasjjrasanen/python-uinput/issues>. However, to avoid
duplicate bug reports, before reporting, please check if similar or identical
bug has already been reported. If so, you can still subscribe to the existing
bug to track its progress.

How to contribute
=================

In addition to reporting bugs and improvement suggestions, you are encouraged to
contribute bug-fixes or features. The source code is maintained in Git and the
main repository is hosted at GitHub,
<https://github.com/tuomasjjrasanen/python-uinput/>. The preferred way to
contribute code is to clone the main Git repository and send a pull-request. Good
old patches via email are also accepted.

How to copy
===========

Python-uinput is free (libre) software and licensed under the terms of GNU
Public License version 3 or later. In short, it means that you are free to copy,
modify and redistribute this software as long as you place the derivative work
under a compatible license. See ``COPYING`` for details.
