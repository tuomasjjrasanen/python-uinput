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

Generate Joystick Input
------------------------

::

    python examples/joystick.py

Use Arrow keys along with W, A, S, D to generate virtual joystick inputs.
If not already installed, you will have to install pygame.

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
