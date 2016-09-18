# -*- coding: utf-8 -*-

import errno
import subprocess

from distutils.core import setup, Extension

libudev_so = "libudev.so.1"

# Because libsuinput can be linked against both libudev.so.0 and
# libudev.so.1, we try to use ldconfig to find out which one is
# available. Preferably libudev.so.1.
try:
    ldconfig_output = subprocess.check_output(["ldconfig", "-p"])
    for line in ldconfig_output.splitlines():

        try:
            lib = line.split()[0]
        except IndexError:
            # An unexpected line, but let's proceed.
            continue

        if lib == "libudev.so.0":
            libudev_so = lib
            # We are quite happy, but let's look if there's something
            # better.

        if lib == "libudev.so.1":
            libudev_so = lib
            break # We are really happy, no reason to look further.
except:
    # We don't care if something goes wrong while we scan through all
    # the available libraries. The whole scan operation is just
    # best-effort, we can always fall back to the default, hard-coded,
    # library.
    pass

setup(name='python-uinput',
      version='0.11.2',
      description='Pythonic API to Linux uinput kernel module.',
      author='Tuomas Räsänen',
      author_email='tuomasjjrasanen@tjjr.fi',
      url='http://tjjr.fi/sw/python-uinput/',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      license='GPLv3+',
      platforms=['Linux'],
      download_url='http://tjjr.fi/sw/python-uinput/releases/python-uinput-0.11.2.tar.gz',
      classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Operating System :: POSIX :: Linux",
        "Topic :: System :: Operating System Kernels :: Linux",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 2.6",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.1",
        "Programming Language :: Python :: 3.2",
        "Programming Language :: Python :: 3.4",
        ],
      long_description="""
Python-uinput is Python interface to Linux uinput kernel module which
allows attaching userspace device drivers into kernel. In practice,
Python-uinput makes it dead simple to create virtual joysticks,
keyboards and mice for generating arbitrary input events
programmatically.
""",
      ext_modules=[Extension('_libsuinput', ['libsuinput/src/suinput.c'],
                             libraries=[":%s" % libudev_so])]
      )
