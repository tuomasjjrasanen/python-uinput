# -*- coding: utf-8 -*-
from __future__ import with_statement
from __future__ import print_function
import re

from distutils.command.build_py import build_py as _build_py
from distutils.core import setup, Extension

def append_ev(ev_type, ev_name):
    with open("/usr/include/linux/input.h") as f:
        with open("src/ev.py", "a") as f2:
            for line in f:
                match = re.match(r"^#define (" + ev_name + "_.*)\t+((?:0x[0-9a-f]+)|(?:\d+))", line)
                if match:
                    print("%s = (%s, %s)" % (match.group(1).strip(), ev_type, match.group(2).strip()), file=f2)

class build_py(_build_py):

    def run(self):
        append_ev("0x01", "KEY")
        append_ev("0x01", "BTN")
        append_ev("0x02", "REL")
        append_ev("0x03", "ABS")
        _build_py.run(self)

setup(name='python-uinput',
      version='0.10.1',
      description='Pythonic API to Linux uinput kernel module.',
      author='Tuomas Räsänen',
      author_email='tuomasjjrasanen@tjjr.fi',
      url='http://tjjr.fi/sw/python-uinput/',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      license='GPLv3+',
      platforms=['Linux'],
      download_url='https://launchpad.net/python-uinput/trunk/0.10.1/+download/python-uinput-0.10.1.tar.gz',
      classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Operating System :: POSIX :: Linux",
        "Topic :: System :: Operating System Kernels :: Linux",
        "Programming Language :: Python :: 2.5",
        "Programming Language :: Python :: 2.6",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.1",
        "Programming Language :: Python :: 3.2",
        ],
      long_description="""
Python-uinput is Python interface to Linux uinput kernel module which
allows attaching userspace device drivers into kernel. In practice,
Python-uinput makes it dead simple to create virtual joysticks,
keyboards and mice for generating arbitrary input events
programmatically.
""",
      cmdclass={'build_py': build_py},
      ext_modules=[Extension('_libsuinput', ['libsuinput/src/suinput.c'],
                             libraries=[":libudev.so.0"])]
      )
