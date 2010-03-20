/*
  evtypes.c - Event types used in Linux input system
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

static PyMethodDef evtypesMethods[] = {
        {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initevtypes(void)
{
        PyObject *module;
        module = Py_InitModule3("evtypes", evtypesMethods,
                                "Event types used in Linux input system");
        PyModule_AddIntMacro(module, EV_SYN);
        PyModule_AddIntMacro(module, EV_KEY);
        PyModule_AddIntMacro(module, EV_REL);
        PyModule_AddIntMacro(module, EV_ABS);
        PyModule_AddIntMacro(module, EV_MSC);
        PyModule_AddIntMacro(module, EV_SW);
        PyModule_AddIntMacro(module, EV_LED);
        PyModule_AddIntMacro(module, EV_SND);
        PyModule_AddIntMacro(module, EV_REP);
        PyModule_AddIntMacro(module, EV_FF);
        PyModule_AddIntMacro(module, EV_PWR);
        PyModule_AddIntMacro(module, EV_FF_STATUS);
        PyModule_AddIntMacro(module, EV_MAX);
        PyModule_AddIntMacro(module, EV_CNT);
}
