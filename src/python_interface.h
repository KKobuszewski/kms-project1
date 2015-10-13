#include <stdio.h>
#include <stdint.h>
#include <Python.h>
#include <numpy/arrayobject.h>

#ifndef __PYTHON_INTERFACE_H__
#define __PYTHON_INTERFACE_H__

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION


/*
 * NOARGS Functions
 */

static PyObject* set_particles(PyObject* self);


/*
 * VARARGS Functions
 */



/*
 * KWARGS Functions
 */

static PyObject* parse_args(PyObject* self, PyObject *args, PyObject *kwargs);


#endif