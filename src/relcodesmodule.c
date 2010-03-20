/*
  relcodes.c - Relative axis codes used in Linux input system
  Copyright (C) 2009 Tuomas Räsänen <tuos@codegrove.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Python.h>
#include <linux/input.h>

static PyMethodDef relcodesMethods[] = {
        {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initrelcodes(void)
{
        PyObject *module;
        module = Py_InitModule3("relcodes", relcodesMethods,
                                "Relative axis codes used in"
                                "Linux input system");
        PyModule_AddIntMacro(module, REL_X);
        PyModule_AddIntMacro(module, REL_Y);
        PyModule_AddIntMacro(module, REL_Z);
        PyModule_AddIntMacro(module, REL_RX);
        PyModule_AddIntMacro(module, REL_RY);
        PyModule_AddIntMacro(module, REL_RZ);
        PyModule_AddIntMacro(module, REL_HWHEEL);
        PyModule_AddIntMacro(module, REL_DIAL);
        PyModule_AddIntMacro(module, REL_WHEEL);
        PyModule_AddIntMacro(module, REL_MISC);
        PyModule_AddIntMacro(module, REL_MAX);
        PyModule_AddIntMacro(module, REL_CNT);
}
