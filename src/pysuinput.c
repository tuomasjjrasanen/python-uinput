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
	uint16_t code;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &code))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_press(c_driver, code) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_release(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t code;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &code))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_release(c_driver, code) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_click(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t code;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &code))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_click(c_driver, code) == -1)
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
	int16_t code;
	if (!PyArg_ParseTuple(args, "Oh", &py_driver, &code))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_press_release(c_driver, code) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_toggle(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t code;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &code))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	if (suinput_toggle(c_driver, code) == -1)
		return PyErr_SetFromErrno(PyExc_IOError);
	Py_RETURN_NONE;
}

static PyObject *pysuinput_is_pressed(PyObject *self, PyObject *args)
{
	PyObject *py_driver;
	struct suinput_driver *c_driver;
	uint16_t code;
	if (!PyArg_ParseTuple(args, "OH", &py_driver, &code))
		return NULL;
	c_driver = (struct suinput_driver *)PyCObject_AsVoidPtr(py_driver);
	return PyBool_FromLong(suinput_is_pressed(c_driver, code));
}

static PyObject *pysuinput_is_valid_code(PyObject *self, PyObject *args)
{
	uint16_t code;
	if (!PyArg_ParseTuple(args, "H", &code))
		return NULL;
	return PyBool_FromLong(suinput_is_valid_code(code));
}

static PyMethodDef pysuinputMethods[] = {
	{"open", pysuinput_open, METH_VARARGS,
	 "open(name, bustype, vendor, product, version)\n\n"
	 "Creates and opens a connection to the event device. Returns an uinput\n"
	 "file descriptor on success. On error, -1 is returned, and errno is set\n"
	 "appropriately.\n"
	 "\n"
	 "All possible values of `bustype` are defined as constants prefixed\n"
	 "by BUS_.\n"   
	},

	{"close", pysuinput_close, METH_VARARGS,
	 "close(driver)\n\n"
	 "Destroys and closes a connection to the event device. Returns 0 on\n"
	 "success. On error, -1 is returned, and errno is set appropriately.\n"
	 "\n"
	 "Behaviour is undefined when passed a file descriptor not returned by\n"
	 "suinput_open()."
	},

	{"click", pysuinput_click, METH_VARARGS,
	 "click(driver, code)\n\n"
	 "Sends a press and release events to the event device. Returns 0 on\n"
	 "success. On error, -1 is returned, and errno is set appropriately.\n"
	 "\n"
	 "Behaviour is undefined when passed a file descriptor not returned by\n"
	 "suinput_open().\n"
	 "\n"
	 "All possible values of `code` are defined as constants in\n"
	 "uinput.codes -module.\n"
	 "\n"
	 "This function is provided as a convenience and has effectively the\n"
	 "same result as calling suinput_press() and suinput_release()\n"
	 "sequentially."
	},

	{"press", pysuinput_press, METH_VARARGS,
	 "press(driver, code)\n\n"
	 "Sends a press event to the event device. Event is repeated after\n"
	 "a short delay until a release event is sent. Returns 0 on success.\n"
	 "On error, -1 is returned, and errno is set appropriately.\n"
	 "\n"
	 "Behaviour is undefined when passed a file descriptor not returned by\n"
	 "suinput_open().\n"
	 "\n"
	 "All possible values of `code` are defined as constants in\n"
	 "uinput.codes -module."
	},

	{"release", pysuinput_release, METH_VARARGS,
	 "release(driver, code)\n\n"
	 "Sends a release event to the event device. Returns 0 on success.\n"
	 "On error, -1 is returned, and errno is set appropriately.\n"
	 "\n"
	 "Behaviour is undefined when passed a file descriptor not returned by\n"
	 "suinput_open().\n"
	 "\n"
	 "All possible values of `code` are defined as constants in\n"
	 "uinput.codes -module."
	},

	{"move_pointer", pysuinput_move_pointer, METH_VARARGS,
	 "move_pointer(driver, x, y)\n\n"
	 "Sends a relative pointer motion event to the event device. Values\n"
	 "increase towards right-bottom. Returns 0 on success. On error, -1\n"
	 "is returned, and errno is set appropriately.\n"
	 "\n"
	 "Behaviour is undefined when passed a file descriptor not returned by\n"
	 "suinput_open()."
	},

	{"press_release", pysuinput_press_release, METH_VARARGS,
	 "press_release(driver, signed_code)\n\n"
	 "Sends a press or a release event to the event device. The sign of\n"
	 "`code` determines which type of event is sent. Positive `code`\n"
	 "means press and negative `code` means release. Returns 0 on\n"
	 "success. On error, -1 is returned, and errno is set appropriately.\n"
	 "\n"
	 "Behaviour is undefined when passed a file descriptor not returned by\n"
	 "suinput_open().\n"
	 "\n"
	 "All possible absolute values of `code` are defined as constants in\n"
	 "uinput.codes -module.\n"
	 "\n"
	 "This function is provided as a convenience and has effectively the\n"
	 "same result as calling suinput_press() when the value of `code` is\n"
	 "positive and suinput_release() when negative."

	},

	{"toggle", pysuinput_toggle, METH_VARARGS,
	 ""
	},

	{"is_pressed", pysuinput_is_pressed, METH_VARARGS,
	 ""
	},

	{"is_valid_code", pysuinput_is_valid_code, METH_VARARGS,
	 ""
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
