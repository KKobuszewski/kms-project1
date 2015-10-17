#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <Python.h>
#include <numpy/arrayobject.h>

#include "python_interface.h"
#include "molecdyn.h"

#define SQRT_3 1.7320508075688773
#define SQRT_2_3 0.816496580927726

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
    {"init_simulation"  , (PyCFunction) parse_args    , METH_VARARGS|METH_KEYWORDS, "doc: test parsing arguments"},
    {"set_particles"    , (PyCFunction) set_particles , METH_NOARGS               , "doc: set_particles"},
    {"end"              , (PyCFunction) end_simulation, METH_NOARGS               , "doc: end simulation"},
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
double a,T0,m,epsilon,f,L,dt;
int nx,ny,nz;
// basis vectors  |  NOTE: Assuming they are normalized!
double base_vec1[] = {       1.,         0.,          0.};
double base_vec2[] = {      0.5, SQRT_3/2.,          0.};
double base_vec3[] = {      0.5, SQRT_3/6., SQRT_2_3};

double *x_arr, *y_arr, *z_arr;
double *vx_arr, *vy_arr, *vz_arr;

void* parsed_args[] = {&a, &nx, &ny, &nz, &T0, &m, &f, &epsilon, &L};

const uint8_t kwlist_lenght = 9; // DO REMEMBER TO UPDATE WHEN CHANGING!
static char *kwlist[] = {"nx", /* number of atoms per dimension, value: 5 */
                         "ny", /* number of atoms per dimension, value: 5 */
                         "nz", /* number of atoms per dimension, value: 5 */
                         "a", /* lattice constant, value: 0.38 */
                         "T0",/* initial temperature, value: 100. */
                         "m", /* mass of a partile, 39.948 */
                         "epsilon", /* minimal energy of Lennard-Jones potential */
                         "f", /* elastic constant, value: 10000.0 */
                         "L", /* bonduary of elastic force */
                         "base_vec_x",
                         "base_vec_y",
                         "base_vec_z",
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
    PyObject *buf1, *buf2, *buf3;
    
    // TODO:
    // 1. Macro
    // 2. Raising Python errors! PyErr_SetString(PyExc_ValueError,"string"); return NULL;
    // 3. Think which arguments are optional and which are required
    //    The arguments are parted by | character!
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iiidddddd|OOO", kwlist, 
                                     &nx, &ny, &nz, &a, &T0, &m, &epsilon, &f, &L,
                                     &buf1,&buf2,&buf3
                                    ))
        return NULL;
    // Parsing args format codes: https://docs.python.org/2.0/ext/parseTuple.html
    
    // TODO: Parse buffers to base vecs!
    
    printf("lattice will be constucted on base vectors: \n");
    printf("(%1.3lf,%1.3lf,%1.3lf)",base_vec1[0],base_vec1[1],base_vec1[2]);
    printf("(%1.3lf,%1.3lf,%1.3lf)",base_vec2[0],base_vec2[1],base_vec2[2]);
    printf("(%1.3lf,%1.3lf,%1.3lf)",base_vec3[0],base_vec3[1],base_vec3[2]);
    printf("\n");
    
    // ========= Init simulation ============== /
    //init_simulation(a, m, T0, f, epsilon, nx, ny, nz, base_vec1, base_vec2, base_vec3);
    
    // print parsed args
    //print_args();
    
    
    
// Returning Py_None
//     Py_INCREF(Py_None);
//     return Py_None;
    Py_RETURN_NONE; // Macro expansio above
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
     * http://stackoverflow.com/questions/27912483/memory-leak-in-python-extension-when-array-is-created-with-pyarray-simplenewfrom
     * http://numpy-discussion.10968.n7.nabble.com/Returning-tuple-of-numpy-arrays-from-a-Numpy-C-extension-td25192.html
     * http://stackoverflow.com/questions/12582875/c-array-to-pyarray
     */
    // common to arrays
    int ndims = 1;
    npy_intp dims[ndims];
    dims[0] = nx*ny*nz;
    
    // print parsed args
    print_args();
    
    
    // ========= Init simulation ============== /
    init_csimulation(a, m, T0, f, epsilon, nx, ny, nz, base_vec1, base_vec2, base_vec3);
    
    // ========= Copy data from simulation ========== /
    //get_data(&x_arr, &y_arr, &z_arr, &vx_arr, &vy_arr, &vz_arr);
    
    // Alloc memory for buffers
    x_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    y_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    z_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    vx_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    vy_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    vz_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    double** positions = return_positions();
    double** velocities = return_velocities();
    
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        x_arr[ii] = positions[0][ii];
        y_arr[ii] = positions[1][ii];
        z_arr[ii] = positions[2][ii];
    }
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        vx_arr[ii] = velocities[0][ii];
        vy_arr[ii] = velocities[1][ii];
        vz_arr[ii] = velocities[2][ii];
    }
    
    // ========= Construct numpy arrays to return ================ /
    // coordinates
    PyArrayObject* py_x_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, x_arr);
    PyArray_ENABLEFLAGS(py_x_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_y_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, y_arr);
    PyArray_ENABLEFLAGS(py_y_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_z_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, z_arr);
    PyArray_ENABLEFLAGS(py_z_arr, NPY_ARRAY_OWNDATA);
    
    // velocities
    PyArrayObject* py_vx_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, vx_arr);
    PyArray_ENABLEFLAGS(py_x_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_vy_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, vy_arr);
    PyArray_ENABLEFLAGS(py_y_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_vz_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, vz_arr);
    PyArray_ENABLEFLAGS(py_z_arr, NPY_ARRAY_OWNDATA);
    
    // clear memory
    // NOTE: Memory for PyArrayObjects and their data will be freed by Python
    free(positions);
    free(velocities);
    
    // TODO: Return dict with results' descriptions <- easier to 
    return Py_BuildValue( "OOOOOO", py_x_arr,
                                    py_y_arr,
                                    py_z_arr,
                                    py_vx_arr,
                                    py_vy_arr,
                                    py_vz_arr );
}

/*
 * To make sure that module and returned np array is initialized
 */
static PyObject* end_simulation(PyObject* self)
{
    free_mem();
    Py_RETURN_NONE;
}