# -*- coding: utf-8 -*-

import os
from distutils.core import setup, Extension

pysuinput_module = Extension('uinput._suinput',
                             sources=['src/pysuinput.c'],
                             libraries=['suinput'],
                             )

codes_module = Extension('uinput.codes',
                         sources=['src/codes.c'])

setup(name='python-uinput',
      version='0.1',
      description='Simple Python API to the Linux uinput-system.',
      long_description='Simple Python API to the Linux uinput-system.',
      author='Tuomas Räsänen',
      author_email='tuos@codegrove.org',
      url='http://codegrove.org/python-uinput/',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      ext_modules=[pysuinput_module, codes_module],
      license='LGPLv3+',
      platforms=['Linux'],
      )
