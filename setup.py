# -*- coding: utf-8 -*-

from distutils.core import setup

setup(name='python-uinput',
      version='0.6',
      description='Python bindings for Linux uinput system.',
      author='Tuomas Jorma Juhani Räsänen',
      author_email='tuomasjjrasanen@tjjr.fi',
      url='http://tjjr.fi/sw/python-uinput/',
      download_url='http://tjjr.fi/sw/python-uinput/src/python-uinput-0.6.tar.gz',
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
      )
