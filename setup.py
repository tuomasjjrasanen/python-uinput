# -*- coding: utf-8 -*-

import os
from distutils.core import setup, Extension

cUinput_module = Extension('cUinput',
                           sources=['src/lib/cUinput.c'],
                           libraries=['suinput'],
                           )

setup(name='python-uinput',
      version='0.1',
      description='Simple Python API to the Linux uinput-system.',
      long_description='Simple Python API to the Linux uinput-system.',
      author='Tuomas Räsänen',
      author_email='tuos@codegrove.org',
      url='http://codegrove.org/python-uinput/',
      package_dir={'': 'src/lib'},
      py_modules=['uinput'],
      ext_modules=[cUinput_module],
      )
