# -*- coding: utf-8 -*-

import os
import sys
reload(sys).setdefaultencoding('utf-8')
import shutil

if os.path.exists('.git'):
    shutil.copy('lib/libsuinput/src/suinput.h', 'include/suinput.h')
    shutil.copy('lib/libsuinput/src/suinput.c', 'src/suinput.c')

from distutils.core import setup, Extension

suinput_module = Extension('uinput.suinput',
                           sources=['src/suinputmodule.c',
                                    'src/suinput.c'],
                           include_dirs=['include'],
                           libraries=['udev'],
                           )

bustypes_module = Extension('uinput.bustypes',
                            sources=['src/bustypesmodule.c'],
                            )

setup(name='python-uinput',
      version='0.3',
      description='Python API to the Linux uinput-system.',
      author='Tuomas Räsänen',
      author_email='tuos@codegrove.org',
      url='http://codegrove.org/python-uinput/',
      download_url='http://codegrove.org/python-uinput/0.3/python-uinput-0.3.tar.gz',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      ext_modules=[suinput_module, bustypes_module],
      license='LGPLv3+',
      platforms=['Linux'],
      classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU Library or Lesser General Public License (LGPL)",
        "Operating System :: POSIX :: Linux",
        "Topic :: System :: Operating System Kernels :: Linux",
        "Programming Language :: Python :: 2.6",
        "Programming Language :: C",
        ],
      long_description="""
A high-level API for generating Linux input events.
""",
      )
