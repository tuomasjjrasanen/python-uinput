# -*- coding: utf-8 -*-

import os
import sys
reload(sys).setdefaultencoding('utf-8')
from distutils.core import setup, Extension

suinput_module = Extension('uinput.suinput',
                           sources=['src/suinputmodule.c'],
                           libraries=['suinput', 'udev'],
                           )

keycodes_module = Extension('uinput.keycodes',
                            sources=['src/keycodesmodule.c'],
                            )

bustypes_module = Extension('uinput.bustypes',
                            sources=['src/bustypesmodule.c'],
                            )

relcodes_module = Extension('uinput.relcodes',
                            sources=['src/relcodesmodule.c'],
                            )

abscodes_module = Extension('uinput.abscodes',
                            sources=['src/abscodesmodule.c'],
                            )

evtypes_module = Extension('uinput.evtypes',
                           sources=['src/evtypesmodule.c'],
                           )

setup(name='python-uinput',
      version='0.3',
      description='Simple Python API to the Linux uinput-system.',
      author='Tuomas Räsänen',
      author_email='tuos@codegrove.org',
      url='http://codegrove.org/python-uinput/',
      download_url='http://codegrove.org/python-uinput/0.3/python-uinput-0.3.tar.gz',
      package_dir={'uinput': 'src'},
      packages=['uinput'],
      ext_modules=[suinput_module, keycodes_module, bustypes_module,
                   relcodes_module, abscodes_module, evtypes_module],
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
