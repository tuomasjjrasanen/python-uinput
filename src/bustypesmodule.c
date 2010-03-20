/*
  bustypes.c - Bustypes used in Linux input system
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

static PyMethodDef bustypesMethods[] = {
        {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initbustypes(void)
{
        PyObject *module;
        module = Py_InitModule3("bustypes", bustypesMethods,
                                "Bustypes used in Linux input system");

        PyModule_AddIntMacro(module, BUS_PCI);
        PyModule_AddIntMacro(module, BUS_ISAPNP);
        PyModule_AddIntMacro(module, BUS_USB);
        PyModule_AddIntMacro(module, BUS_HIL);
        PyModule_AddIntMacro(module, BUS_BLUETOOTH);
        PyModule_AddIntMacro(module, BUS_VIRTUAL);
        PyModule_AddIntMacro(module, BUS_ISA);
        PyModule_AddIntMacro(module, BUS_I8042);
        PyModule_AddIntMacro(module, BUS_XTKBD);
        PyModule_AddIntMacro(module, BUS_RS232);
        PyModule_AddIntMacro(module, BUS_GAMEPORT);
        PyModule_AddIntMacro(module, BUS_PARPORT);
        PyModule_AddIntMacro(module, BUS_AMIGA);
        PyModule_AddIntMacro(module, BUS_ADB);
        PyModule_AddIntMacro(module, BUS_I2C);
        PyModule_AddIntMacro(module, BUS_HOST);
        PyModule_AddIntMacro(module, BUS_GSC);
        PyModule_AddIntMacro(module, BUS_ATARI);
}
