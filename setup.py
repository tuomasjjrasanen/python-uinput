# -*- coding: utf-8 -*-

import re

from distutils.command.build_py import build_py as _build_py
from distutils.core import setup, Extension

def append_ev(ev_type, ev_name):
    with open("/usr/include/linux/input.h") as f:
        with open("src/ev.py", "a") as f2:
            for line in f:
                match = re.match(r"^#define (" + ev_name + "_.*)\t+((?:0x[0-9a-f]+)|(?:\d+))", line)
                if match:
                    print >>f2, "%s = (%s, %s)" % (match.group(1).strip(), ev_type, match.group(2).strip())

class build_py(_build_py):

    def run(self):
        append_ev("0x01", "KEY")
        append_ev("0x01", "BTN")
        append_ev("0x02", "REL")
        append_ev("0x03", "ABS")
        _build_py.run(self)

setup(name='python-uinput',
      version='0.6',
      description='Python bindings for Linux uinput system.',
      author='Tuomas Jorma Juhani Räsänen',
      author_email='tuomasjjrasanen@tjjr.fi',
      url='http://tjjr.fi/sw/python-uinput/',
      download_url='http://launchpad.net/python-uinput/trunk/0.6/+download/python-uinput-0.6.tar.gz',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      license='GPLv3+',
      platforms=['Linux'],
      classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Operating System :: POSIX :: Linux",
        "Topic :: System :: Operating System Kernels :: Linux",
        "Programming Language :: Python :: 2.5",
        "Programming Language :: Python :: 2.6",
        ],
      long_description="""
Uinput is a Linux kernel module which allows attaching user-space
device drivers into the Linux kernel. Python-uinput provides a
pythonic API to the Linux uinput system.
""",
      cmdclass={'build_py': build_py},
      ext_modules=[Extension('_libsuinput', ['libsuinput/src/suinput.c'],
                             libraries=["udev"])]
      )
