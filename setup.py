# -*- coding: utf-8 -*-

import os
import sys

from distutils.core import setup, Extension

suinput_module = Extension('uinput.suinput',
                           sources=['src/suinputmodule.c'],
                           libraries=['suinput'],
                           )

bustypes_module = Extension('uinput.bustypes',
                            sources=['src/bustypesmodule.c'],
                            )

setup(name='python-uinput',
      version='0.5',
      description='Create Linux input device drivers with Python.',
      author='Tuomas Jorma Juhani Räsänen',
      author_email='tuomas.j.j.rasanen@tjjr.fi',
      url='http://codegrove.org/projects/python-uinput/',
      download_url='http://pypi.python.org/packages/source/p/python-uinput/python-uinput-0.5.tar.gz',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      ext_modules=[suinput_module, bustypes_module],
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
        "Programming Language :: C",
        ],
      long_description="""
Uinput allows attaching user-space device drivers into the Linux
kernel. Python-uinput provides a simple and easy to use API to the
Linux uinput-system.
""",
      )
