# -*- coding: utf-8 -*-

import os
import sys
reload(sys).setdefaultencoding('utf-8')

from distutils.core import setup, Extension

suinput_module = Extension('uinput.suinput',
                           sources=['src/suinputmodule.c', 'src/suinput.c'],
                           include_dirs=['include'],
                           libraries=['udev'],
                           )

bustypes_module = Extension('uinput.bustypes',
                            sources=['src/bustypesmodule.c'],
                            )

setup(name='python-uinput',
      version='0.3.2',
      description='Python API to the Linux uinput-system.',
      author='Tuomas Räsänen',
      author_email='tuos@codegrove.org',
      url='http://codegrove.org/python-uinput/',
      download_url='http://codegrove.org/python-uinput/releases/python-uinput-0.3.2.tar.gz',
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
        "Programming Language :: Python :: 2.6",
        "Programming Language :: C",
        ],
      long_description="""
A high-level API for generating Linux input events.
""",
      )
