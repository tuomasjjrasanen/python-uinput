/*
  pysuinput - Simple Python wrapper for libsuinput
  Copyright © 2010 Tuomas Räsänen (tuos) <tuos@codegrove.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <Python.h>

#include <suinput.h>

static PyObject *pysuinput_set_capabilities(PyObject *self, PyObject *args)
{
    int uinput_fd;
    uint16_t type;
    PyObject *capabilities;
    int capability_err;
    PyObject *iter;
    PyObject *capability;
    int *codes;
    int i;

    if (!PyArg_ParseTuple(args, "IHO!", &uinput_fd, &type,
                          &PySet_Type, &capabilities))
        return NULL;

    codes = (int *) calloc(PySet_Size(capabilities), sizeof(int));

    if ((iter = PyObject_GetIter(capabilities)) == NULL) {
        free(codes);
        return NULL;
    }

    i = 0;
    while ((capability = PyIter_Next(iter))) {
        codes[i] = (int) PyInt_AsLong(capability);
        Py_DECREF(capability);
        if (codes[i] == -1 && PyErr_Occurred())
            break;
        ++i;
    }

    Py_DECREF(iter);

    if (PyErr_Occurred()) {
        free(codes);
        return NULL;
    }

    capability_err = suinput_set_capabilities(uinput_fd, type,
                                              codes, i);

    free(codes);
    codes = NULL;

    if (capability_err == -1) {
        return PyErr_SetFromErrno(PyExc_IOError);
    } else if (capability_err == -2) {
        PyErr_SetString(PyExc_ValueError, "Unsupported event type.");
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *pysuinput_syn(PyObject *self, PyObject *args)
{
    int uinput_fd;
    if (!PyArg_ParseTuple(args, "I", &uinput_fd))
        return NULL;
    if (suinput_syn(uinput_fd) == -1)
        PyErr_SetFromErrno(PyExc_IOError);
    Py_RETURN_NONE;
}

int from_list_to_carray(PyObject *tuple, int *arr)
{
    PyObject *iter;
    PyObject *item;
    int i;

    if ((iter = PyObject_GetIter(tuple)) == NULL)
        return -1;

    i = 0;
    while ((item = PyIter_Next(iter))) {
        arr[i] = (int) PyInt_AsLong(item);
        Py_DECREF(item);
        if (arr[i] == -1 && PyErr_Occurred())
            break;
        ++i;
    }

    Py_DECREF(iter);

    if (PyErr_Occurred())
        return -1;

    return 0;
}

static PyObject *pysuinput_create(PyObject *self, PyObject *args)
{
    int uinput_fd;
    const char *name = NULL;
    uint16_t bustype;
    uint16_t vendor;
    uint16_t product;
    uint16_t version;
    int ff_effects_max;
    PyObject *absmin;
    PyObject *absmax;
    PyObject *absfuzz;
    PyObject *absflat;

    if (!PyArg_ParseTuple(args, "IsHHHHIO!O!O!O!", &uinput_fd, &name,
                          &bustype,
                          &vendor, &product, &version, &ff_effects_max,
                          &PyList_Type, &absmin,
                          &PyList_Type, &absmax,
                          &PyList_Type, &absfuzz,
                          &PyList_Type, &absflat))
        return NULL;
    struct uinput_user_dev user_dev;
    memset(&user_dev, 0, sizeof(struct uinput_user_dev));
    strcpy(user_dev.name, name);
    user_dev.id.bustype = bustype;
    user_dev.id.vendor = vendor;
    user_dev.id.product = product;
    user_dev.id.version = version;
    user_dev.ff_effects_max = ff_effects_max;

    if (from_list_to_carray(absmin, user_dev.absmin) == -1)
        return NULL;
    if (from_list_to_carray(absmax, user_dev.absmax) == -1)
        return NULL;
    if (from_list_to_carray(absfuzz, user_dev.absfuzz) == -1)
        return NULL;
    if (from_list_to_carray(absflat, user_dev.absflat) == -1)
        return NULL;
    if (suinput_create(uinput_fd, &user_dev) == -1)
        PyErr_SetFromErrno(PyExc_IOError);
    Py_RETURN_NONE;
}

static PyObject *pysuinput_write(PyObject *self, PyObject *args)
{
    int uinput_fd;
    uint16_t type;
    uint16_t code;
    int32_t value;
    if (!PyArg_ParseTuple(args, "IHHI", &uinput_fd, &type, &code, &value))
        return NULL;
    if (suinput_write(uinput_fd, type, code, value) == -1)
        return PyErr_SetFromErrno(PyExc_IOError);
    Py_RETURN_NONE;
}

static PyObject *pysuinput_open(PyObject *self, PyObject *args)
{
    int uinput_fd;
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    if ((uinput_fd = suinput_open()) == -1)
        return PyErr_SetFromErrno(PyExc_IOError);
    return PyInt_FromLong(uinput_fd);
}

static PyObject *pysuinput_destroy(PyObject *self, PyObject *args)
{
    int uinput_fd;
    if (!PyArg_ParseTuple(args, "I", &uinput_fd))
        return NULL;
    if (suinput_destroy(uinput_fd) == -1)
        return PyErr_SetFromErrno(PyExc_IOError);
    Py_RETURN_NONE;
}

static PyMethodDef pysuinputMethods[] = {
    {"uinput_open", pysuinput_open, METH_VARARGS,
     "uinput_open()\n\n"
     "Open the uinput device and return a file descriptor."
    },

    {"uinput_write", pysuinput_write, METH_VARARGS,
     "uinput_write(uinput_fd, ev_type, ev_code, ev_value)\n\n"
     "Destory and close the uinput device.\n"
    },

    {"uinput_set_capabilities", pysuinput_set_capabilities, METH_VARARGS,
     "uinput_set_capabilities(uinput_fd, ev_type, capabilities)\n\n"
     "Destory and close the uinput device.\n"
    },

    {"uinput_syn", pysuinput_syn, METH_VARARGS,
     "uinput_syn(uinput_fd, ev_type, ev_code, ev_value)\n\n"
     "Destory and close the uinput device.\n"
    },

    {"uinput_destroy", pysuinput_destroy, METH_VARARGS,
     "uinput_destroy(uinput_fd)\n\n"
     "Destory and close the uinput device.\n"
    },

    {"uinput_create", pysuinput_create, METH_VARARGS,
     "uinput_create(uinput_fd, name, bustype, vendor, product, version,\n"
     "              ff_effects_max, absmin, absmax, absfuzz, absflat)\n\n"
     "Destory and close the uinput device.\n"
    },

    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initsuinput(void)
{
    PyObject *module;
    module = Py_InitModule3("suinput", pysuinputMethods,
                            "Simple Python wrapper for libsuinput");
}
