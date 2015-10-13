#include <stdio.h>
#include <stdint.h>
#include <Python.h>
#include <numpy/arrayobject.h>

#include "python_interface.h"

/* *************************************************************************************************************************** *
 *                                                                                                                             *
 *                                                     INITIALIZATION                                                          *
 *                                                                                                                             *
 * *************************************************************************************************************************** */

static PyMethodDef module_methods[] = {
    /* The cast of the function is necessary since PyCFunction values
     * only take two PyObject* parameters, and keywdarg_parrot() takes
     * three.
     */
    {"init"             , (PyCFunction) parse_args    , METH_VARARGS|METH_KEYWORDS, "doc: test parsing arguments"},
    {"set_particles"    , (PyCFunction) set_particles , METH_NOARGS               , "doc: set_particles"},
    { NULL, NULL, 0, NULL }   /* sentinel */
};

#ifndef _PYTHON3
PyMODINIT_FUNC initMolecDyn(void)
{
  /* Create the module and add the functions */
  Py_InitModule3("MolecDyn", module_methods,"Module for molecular dynamics simulation");
  
  /* Load numpy arrays API */
  import_array();
  
}

#else

static struct PyModuleDef MolecDynDef =
{
    PyModuleDef_HEAD_INIT,
    "MolecDyn",  /* name of module */
    "Module for molecular dynamics simulation",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    module_methods
};

PyMODINIT_FUNC PyInit_MolecDyn(void)
{
  /* Load numpy arrays API */
  import_array();
  
  /* Create the module and add the functions */
  return PyModule_Create(&MolecDynDef);
}
#endif


/* *************************************************************************************************************************** *
 *                                                                                                                             *
 *                                               SET SIMULATION PARAMETERS                                                     *
 *                                                                                                                             *
 * *************************************************************************************************************************** */

// Declare global params to parse    
double a,T0,m,f;
int n;
void* parsed_args[] = {&a, &n, &T0, &m, &f};

const uint8_t kwlist_lenght = 5; // DO REMEMBER TO UPDATE WHEN CHANGING!
static char *kwlist[] = {"a", /* lattice constant, value: 0.38 */
                         "n", /* number of atoms per dimension, value: 5 */
                         "T0",/* initial temperature, value: 100. */
                         "m", /* mass of a partile, 39.948 */
                         "f", /* elastic constant, value: 10000.0 */
                         NULL};

static inline void print_args(void)
{
    for (uint8_t ii = 0; ii < kwlist_lenght; ii++)
    {
        printf("%s: %lf\n", kwlist[ii],*((double*) parsed_args[ii])); // C is beautiful!
    }
}
                         
static PyObject* parse_args(PyObject* self, PyObject *args, PyObject *kwargs)
{
    
    
    // TODO:
    // 1. Macro
    // 2. Raising Python errors! PyErr_SetString(PyExc_ValueError,"string"); return NULL;
    // 3. Think which arguments are optional and which are required
    //    The arguments are parted by | character!
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "diddd", kwlist, 
                                     &a, &n, &T0, &m, &f))
        return NULL;
    
    // Parsing args format codes: https://docs.python.org/2.0/ext/parseTuple.html
    
    // print parsed args
    print_args();
    
    
    // TODO: define a marco or use Py_RETURN_NONE
    Py_INCREF(Py_None);
    return Py_None;
}



/* *************************************************************************************************************************** *
 *                                                                                                                             *
 *                                              SET PARTICLES IN A LATTICE                                                     *
 *                                                                                                                             *
 * *************************************************************************************************************************** */

static PyObject* set_particles(PyObject* self)
{
    /*
     * More tutorials:
     * http://dan.iel.fm/posts/python-c-extensions/
     * http://chimera.labs.oreilly.com/books/1230000000393/ch15.html#_solution_241
     * http://folk.uio.no/inf3330/scripting/doc/python/NumPy/Numeric/numpy-13.html
     */
    // common to arrays
    int ndims = 1;
    npy_intp dims[ndims];
    dims[0] = n*n*n;
    
    // coordinates
    double* x_arr = (double*) malloc( sizeof(double) * n*n*n );
    PyArrayObject* py_x_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, x_arr);
    PyArray_ENABLEFLAGS(py_x_arr, NPY_ARRAY_OWNDATA);
    // folowing http://stackoverflow.com/questions/27912483/memory-leak-in-python-extension-when-array-is-created-with-pyarray-simplenewfrom
    
    double* y_arr = (double*) malloc( sizeof(double) * n*n*n );
    PyArrayObject* py_y_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, y_arr);
    PyArray_ENABLEFLAGS(py_y_arr, NPY_ARRAY_OWNDATA);
    
    double* z_arr = (double*) malloc( sizeof(double) * n*n*n );
    PyArrayObject* py_z_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, z_arr);
    PyArray_ENABLEFLAGS(py_z_arr, NPY_ARRAY_OWNDATA);
    
    
    // http://numpy-discussion.10968.n7.nabble.com/Returning-tuple-of-numpy-arrays-from-a-Numpy-C-extension-td25192.html
    // http://stackoverflow.com/questions/12582875/c-array-to-pyarray
    return Py_BuildValue( "OOO", py_x_arr,
                                 py_y_arr,
                                 py_z_arr  );
}