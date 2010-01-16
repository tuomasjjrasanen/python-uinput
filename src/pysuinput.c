/*
  pysuinput - Simple Python wrapper for libsuinput
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

#include "suinput.h"

static PyObject *pysuinput_open(PyObject *self, PyObject *args)
{
	const char *name = NULL;
	uint16_t bustype;
	uint16_t vendor;
	uint16_t product;
	uint16_t version;
	if (!PyArg_ParseTuple(args, "sHHHH", &name, &bustype, &vendor, &product,
			      &version))
		return NULL;
	struct input_id id = {bustype, vendor, product, version};
	struct suinput_driver *c_driver = suinput_open(name, &id);
	if (c_driver == NULL)
		return PyErr_SetFromErrno(PyExc_IOError);
	return PyCObject_FromVoidPtr((void *)c_driver, NULL);
}

static PyObject *pysuinput_close(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	if (!PyArg_ParseTuple(args, "O", &py_driver))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_close(c_driver) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_press(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t keycode;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &keycode))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_press(c_driver, keycode) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_release(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t keycode;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &keycode))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_release(c_driver, keycode) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_click(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t keycode;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &keycode))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_click(c_driver, keycode) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_move_pointer(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	int32_t x;
	int32_t y;
	if (!PyArg_ParseTuple(args, "Oii", &py_driver, &x, &y)) {
		return NULL;
	}
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_move_pointer(c_driver, x, y) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_press_release(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	int16_t keycode;
	if (!PyArg_ParseTuple(args, "Oh", &py_driver, &keycode))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_press_release(c_driver, keycode) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_toggle(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t keycode;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &keycode))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_toggle(c_driver, keycode) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_is_pressed(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t keycode;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &keycode))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	return PyBool_FromLong(suinput_is_pressed(c_driver, keycode));
}

static PyMethodDef pysuinputMethods[] = {
	{"open", pysuinput_open, METH_VARARGS,
	 "open(name, bustype, vendor, product, version)\n\n"
	 "Return an input driver.\n"
	 "All possible values of bustype are defined as constants prefixed\n"
	 "by BUS_, vendor, product and version are unsigned 16 bit integers."
	 "close() must be called for every object returned by this function."
	},

	{"close", pysuinput_close, METH_VARARGS,
	 "close(driver)\n\n"
	 "Close and destroy the driver."
	},

	{"click", pysuinput_click, METH_VARARGS,
	 "click(driver, keycode)\n\n"
	 "Send a press and a release event."
	},

	{"press", pysuinput_press, METH_VARARGS,
	 "press(driver, keycode)\n\n"
	 "Send a press event.\n"
	 "Event is repeated after a short delay until a release event is sent."
	},

	{"release", pysuinput_release, METH_VARARGS,
	 "release(driver, keycode)\n\n"
	 "Send a release event."
	},

	{"move_pointer", pysuinput_move_pointer, METH_VARARGS,
	 "move_pointer(driver, x, y)\n\n"
	 "Move pointer towards bottom-right."
	},

	{"press_release", pysuinput_press_release, METH_VARARGS,
	 "press_release(driver, signed_keycode)\n\n"
	 "Send a press event if signed_keycode > 0, otherwise send\n"
	 "a release event."
	},

	{"toggle", pysuinput_toggle, METH_VARARGS,
	 "toggle(driver, keycode)\n\n"
	 "Press button if it is not pressed currently, release it otherwise."
	},

	{"is_pressed", pysuinput_is_pressed, METH_VARARGS,
	 "is_pressed(driver, keycode)\n\n"
	 "Return True if button is pressed, False otherwise.\n"
	 "keycode must be one of the constant values defined in\n"
	 "uinput.keycodes -module."
	},

	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_suinput(void)
{
	PyObject *module;
	module = Py_InitModule3("_suinput", pysuinputMethods,
				"Simple C-like Python API to the Linux uinput-system\n\n"
				"Provides a high-level API for the Linux input subsystem\n"
				"through the user space event device.");
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
