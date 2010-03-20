/*
  abscodes.c - Asbolute axis codes used in Linux input system
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

static PyMethodDef abscodesMethods[] = {
        {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initabscodes(void)
{
        PyObject *module;
        module = Py_InitModule3("abscodes", abscodesMethods,
                                "Asbolute axis codes used in Linux input system");

        PyModule_AddIntMacro(module, ABS_X);
        PyModule_AddIntMacro(module, ABS_Y);
        PyModule_AddIntMacro(module, ABS_Z);
        PyModule_AddIntMacro(module, ABS_RX);
        PyModule_AddIntMacro(module, ABS_RY);
        PyModule_AddIntMacro(module, ABS_RZ);
        PyModule_AddIntMacro(module, ABS_THROTTLE);
        PyModule_AddIntMacro(module, ABS_RUDDER);
        PyModule_AddIntMacro(module, ABS_WHEEL);
        PyModule_AddIntMacro(module, ABS_GAS);
        PyModule_AddIntMacro(module, ABS_BRAKE);
        PyModule_AddIntMacro(module, ABS_HAT0X);
        PyModule_AddIntMacro(module, ABS_HAT0Y);
        PyModule_AddIntMacro(module, ABS_HAT1X);
        PyModule_AddIntMacro(module, ABS_HAT1Y);
        PyModule_AddIntMacro(module, ABS_HAT2X);
        PyModule_AddIntMacro(module, ABS_HAT2Y);
        PyModule_AddIntMacro(module, ABS_HAT3X);
        PyModule_AddIntMacro(module, ABS_HAT3Y);
        PyModule_AddIntMacro(module, ABS_PRESSURE);
        PyModule_AddIntMacro(module, ABS_DISTANCE);
        PyModule_AddIntMacro(module, ABS_TILT_X);
        PyModule_AddIntMacro(module, ABS_TILT_Y);
        PyModule_AddIntMacro(module, ABS_TOOL_WIDTH);
        PyModule_AddIntMacro(module, ABS_VOLUME);
        PyModule_AddIntMacro(module, ABS_MISC);
        PyModule_AddIntMacro(module, ABS_MT_TOUCH_MAJOR);
        PyModule_AddIntMacro(module, ABS_MT_TOUCH_MINOR);
        PyModule_AddIntMacro(module, ABS_MT_WIDTH_MAJOR);
        PyModule_AddIntMacro(module, ABS_MT_WIDTH_MINOR);
        PyModule_AddIntMacro(module, ABS_MT_ORIENTATION);
        PyModule_AddIntMacro(module, ABS_MT_POSITION_X);
        PyModule_AddIntMacro(module, ABS_MT_POSITION_Y);
        PyModule_AddIntMacro(module, ABS_MT_TOOL_TYPE);
        PyModule_AddIntMacro(module, ABS_MT_BLOB_ID);
        PyModule_AddIntMacro(module, ABS_MT_TRACKING_ID);
        PyModule_AddIntMacro(module, ABS_MAX);
        PyModule_AddIntMacro(module, ABS_CNT);
}
