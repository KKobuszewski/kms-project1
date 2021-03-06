#include <stdio.h>
#include <stdint.h>
#include <Python.h>
#include <numpy/arrayobject.h>

#ifndef __PYTHON_INTERFACE_H__
#define __PYTHON_INTERFACE_H__

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

/*
 * MACRO FUNCTIONS
 */
#define CheckPyErr(cmd,message)



/*
 * NOARGS Functions
 */

static PyObject* set_particles(PyObject* self);
static PyObject* py_reset_lattice(PyObject* self);
static PyObject* py_termalize(PyObject* self);
static PyObject* end_simulation(PyObject* self);
static PyObject* stats(PyObject* self);
static PyObject* get_positions(PyObject* self);
static PyObject* get_momenta(PyObject* self);
static PyObject* get_forces(PyObject* self);


/*
 * VARARGS Functions
 */

static PyObject* py_change_R(PyObject* self, PyObject *args);
static PyObject* py_change_a(PyObject* self, PyObject *args);
static PyObject* py_change_T(PyObject* self, PyObject *args);

static PyObject* simulate(PyObject* self, PyObject *args);


/*
 * KWARGS Functions
 */

static PyObject* parse_args(PyObject* self, PyObject *args, PyObject *kwargs);
static PyObject* evolve(PyObject* self, PyObject *args, PyObject *kwargs);



/*
 * Internal functions
 */

static inline void print_args(void);


#endif