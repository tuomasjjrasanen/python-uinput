# -*- coding: utf-8 -*-

from distutils.core import setup, Extension

setup(name='python-uinput',
      version='0.11.0',
      description='Pythonic API to Linux uinput kernel module.',
      author='Tuomas Räsänen',
      author_email='tuomasjjrasanen@tjjr.fi',
      url='http://tjjr.fi/sw/python-uinput/',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      license='GPLv3+',
      platforms=['Linux'],
      download_url='http://tjjr.fi/sw/python-uinput/releases/python-uinput-0.11.0.tar.gz',
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
                             libraries=[":libudev.so"])]
      )
