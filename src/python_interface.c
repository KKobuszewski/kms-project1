#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
    {"system_stats"     , (PyCFunction) stats         , METH_NOARGS               , "doc: statistics of system in current time"},
    {"evolve"           , (PyCFunction) evolve        , METH_VARARGS|METH_KEYWORDS, "doc: evolves system t with timestep dt"},
    {"get_positions"    , (PyCFunction) get_positions , METH_NOARGS               , "doc: returns positions of particles, new numpy array for each dimension"},
    {"get_momenta"      , (PyCFunction) get_momenta   , METH_NOARGS               , "doc: returns momenta of particles, new numpy array for each dimension"},
    {"get_forces"       , (PyCFunction) get_forces    , METH_NOARGS               , "doc: returns forces of particles, new numpy array for each dimension"},
    {"end"              , (PyCFunction) end_simulation, METH_NOARGS               , "doc: ends simulation and frees mem"},
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
double a;
double T0,m,epsilon,f,L,dt;
int nx,ny,nz;
// basis vectors  |  NOTE: Assuming they are normalized!
double base_vec1[] = {       1.,          0.,          0.};
double base_vec2[] = {      0.5,   SQRT_3/2.,          0.};
double base_vec3[] = {      0.5,   SQRT_3/6.,    SQRT_2_3};


void* parsed_args[] = {&nx, &ny, &nz, &a, &T0, &m, &epsilon, &f, &L, &dt};

const uint8_t kwlist_lenght = 10; // DO REMEMBER TO UPDATE WHEN CHANGING!
static char *kwlist[] = {"nx", /* number of atoms per dimension, value: 5 */
                         "ny", /* number of atoms per dimension, value: 5 */
                         "nz", /* number of atoms per dimension, value: 5 */
                         "a", /* lattice constant, value: 0.38 */
                         "T0",/* initial temperature, value: 100. */
                         "m", /* mass of a partile, 39.948 */
                         "epsilon", /* minimal energy of Lennard-Jones potential */
                         "f", /* elastic constant, value: 10000.0 */
                         "L", /* bonduary of elastic force */
                         "dt",
                         "base_vec_x",
                         "base_vec_y",
                         "base_vec_z",
                         NULL};

#pragma GCC push_options
#pragma GCC optimize ("O0")
static inline void print_args(void)
{
    for (uint8_t ii = 0; ii < kwlist_lenght; ii++)
    {
        if( ii==0 || ii==1 || ii==2 )
            printf("%s: %d\n", kwlist[ii],*((int*) parsed_args[ii]));
        else
            printf("%s: %3.3lf\n", kwlist[ii],*((double*) parsed_args[ii]));
    }
}
#pragma GCC pop_options

static PyObject* parse_args(PyObject* self, PyObject *args, PyObject *kwargs)
{
    PyObject *buf1, *buf2, *buf3;
    
    // TODO:
    // 1. Macro
    // 2. Raising Python errors! PyErr_SetString(PyExc_ValueError,"string"); return NULL;
    // 3. Think which arguments are optional and which are required
    //    The arguments are parted by | character!
    if ( !PyArg_ParseTupleAndKeywords(args, kwargs, "iiiddddddd|OOO", kwlist, 
                                     &nx, &ny, &nz, &a, &T0, &m, &epsilon, &f, &L, &dt,
                                     &buf1,&buf2,&buf3) )
        return NULL;
    // Parsing args format codes: https://docs.python.org/2.0/ext/parseTuple.html
    
    // TODO: Parse buffers to base vecs!
    
//     printf("lattice will be constucted on base vectors: \n");
//     printf("(%1.3lf,%1.3lf,%1.3lf)",base_vec1[0],base_vec1[1],base_vec1[2]);
//     printf("(%1.3lf,%1.3lf,%1.3lf)",base_vec2[0],base_vec2[1],base_vec2[2]);
//     printf("(%1.3lf,%1.3lf,%1.3lf)",base_vec3[0],base_vec3[1],base_vec3[2]);
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
    
//     printf("constructing lattice %ux%ux%u\n",nx,ny,nz);
//     
//     printf("a:  %lf\nnx: %d\nny: %d\nnz: %d\n",a,nx,ny,nz);
    
    
    // ========= Init simulation ============== /
    init_csimulation(a, m, T0, epsilon, f, L, dt, nx, ny, nz, base_vec1, base_vec2, base_vec3);
    
    // ========= Copy data from simulation ========== /
    //get_data(&x_arr, &y_arr, &z_arr, &px_arr, &py_arr, &pz_arr);
    
    
    double *x_arr, *y_arr, *z_arr;
    double *px_arr, *py_arr, *pz_arr;
    
    // Alloc memory for buffers
    x_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    y_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    z_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    px_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    py_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    pz_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    double** positions = return_positions();
    double** momenta = return_momenta();
    
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        x_arr[ii] = positions[0][ii];
        y_arr[ii] = positions[1][ii];
        z_arr[ii] = positions[2][ii];
    }
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        px_arr[ii] = momenta[0][ii];
        py_arr[ii] = momenta[1][ii];
        pz_arr[ii] = momenta[2][ii];
    }
    
    // ========= Construct numpy arrays to return ================ /
    // coordinates
    PyArrayObject* py_x_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, x_arr);
    PyArray_ENABLEFLAGS(py_x_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_y_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, y_arr);
    PyArray_ENABLEFLAGS(py_y_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_z_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, z_arr);
    PyArray_ENABLEFLAGS(py_z_arr, NPY_ARRAY_OWNDATA);
    
    // momenta
    PyArrayObject* py_px_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, px_arr);
    PyArray_ENABLEFLAGS(py_x_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_py_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, py_arr);
    PyArray_ENABLEFLAGS(py_y_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_pz_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, pz_arr);
    PyArray_ENABLEFLAGS(py_z_arr, NPY_ARRAY_OWNDATA);
    
    // clear memory
    // NOTE: Memory for PyArrayObjects and their data will be freed by Python
    free(positions);
    free(momenta);
    
    // TODO: Return dict with results' descriptions <- easier to 
    return Py_BuildValue( "OOOOOO", py_x_arr,
                                    py_y_arr,
                                    py_z_arr,
                                    py_px_arr,
                                    py_py_arr,
                                    py_pz_arr );
}


/* *************************************************************************************************************************** *
 *                                                                                                                             *
 *                                              SIMULATION RUNTIME METHODS                                                     *
 *                                                                                                                             *
 * *************************************************************************************************************************** */
/*
 * returns numpy array of system's macroparameters in order T_ENERGY,LJ_POTENTIAL, SP_POTENTIAL, PRESSURE, ISNT_TEMP
 */
static PyObject* stats(PyObject* self)
{
    int ndims = 1;
    npy_intp dims[ndims];
    dims[0] = (INST_TEMP+1);
    
    double* stats = get_statistics();
    
//     printf("Python binding\n");
//     printf("kinetic: %lf\n",stats[0]);
//     printf("LJ     : %lf\n",stats[1]);
//     printf("spring : %lf\n",stats[2]);
//     printf("press. : %lf\n",stats[3]);
//     printf("temp.  : %lf\n",stats[4]);
    
    PyArrayObject* py_stats = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, stats);
    PyArray_ENABLEFLAGS(py_stats, NPY_ARRAY_OWNDATA);
    
    return Py_BuildValue( "O", py_stats );
}

static PyObject* get_positions(PyObject* self)
{
    double *x_arr, *y_arr, *z_arr;
    
    int ndims = 1;
    npy_intp dims[ndims];
    dims[0] = nx*ny*nz;
    
    // Alloc memory for buffers
    x_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    y_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    z_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    double** positions = return_positions();
    
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        x_arr[ii] = positions[0][ii];
        y_arr[ii] = positions[1][ii];
        z_arr[ii] = positions[2][ii];
    }
    
    // ========= Construct numpy arrays to return ================ /
    PyArrayObject* py_x_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, x_arr);
    PyArray_ENABLEFLAGS(py_x_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_y_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, y_arr);
    PyArray_ENABLEFLAGS(py_y_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_z_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, z_arr);
    PyArray_ENABLEFLAGS(py_z_arr, NPY_ARRAY_OWNDATA);
    
    return Py_BuildValue( "OOO", py_x_arr, py_y_arr, py_z_arr );
}

static PyObject* get_momenta(PyObject* self)
{
    double *px_arr, *py_arr, *pz_arr;
    
    int ndims = 1;
    npy_intp dims[ndims];
    dims[0] = nx*ny*nz;
    
    px_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    py_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    pz_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    double** momenta = return_momenta();
    
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        px_arr[ii] = momenta[0][ii];
        py_arr[ii] = momenta[1][ii];
        pz_arr[ii] = momenta[2][ii];
    }
    
    // ========= Construct numpy arrays to return ================ /
    PyArrayObject* py_px_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, px_arr);
    PyArray_ENABLEFLAGS(py_px_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_py_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, py_arr);
    PyArray_ENABLEFLAGS(py_py_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_pz_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, pz_arr);
    PyArray_ENABLEFLAGS(py_pz_arr, NPY_ARRAY_OWNDATA);
    
    return Py_BuildValue( "OOO", py_px_arr, py_py_arr, py_pz_arr );
}

static PyObject* get_forces(PyObject* self)
{
    double *fx_arr, *fy_arr, *fz_arr;
    
    int ndims = 1;
    npy_intp dims[ndims];
    dims[0] = nx*ny*nz;
    
    fx_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    fy_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    fz_arr = (double*) malloc( sizeof(double) * nx*ny*nz );
    
    double** forces = return_forces();
    
    for (int ii = 0; ii < nx*ny*nz; ii++)
    {
        fx_arr[ii] = forces[0][ii];
        fy_arr[ii] = forces[1][ii];
        fz_arr[ii] = forces[2][ii];
    }
    
    // ========= Construct numpy arrays to return ================ /
    PyArrayObject* py_fx_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, fx_arr);
    PyArray_ENABLEFLAGS(py_fx_arr, NPY_ARRAY_OWNDATA);
        
    PyArrayObject* py_fy_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, fy_arr);
    PyArray_ENABLEFLAGS(py_fy_arr, NPY_ARRAY_OWNDATA);
    
    PyArrayObject* py_fz_arr = (PyArrayObject *) PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, fz_arr);
    PyArray_ENABLEFLAGS(py_fz_arr, NPY_ARRAY_OWNDATA);
    
    return Py_BuildValue( "OOO", py_fx_arr, py_fy_arr, py_fz_arr );
}


static char* evolve_kwlist[] = {"steps","dt"};
/*
 * Evolves system with given number of steps and timestep lenght.
 */
static PyObject* evolve(PyObject* self, PyObject *args, PyObject *kwargs)
{
    
    
    int steps;
    double dt_new = -1.;
    
    if ( !PyArg_ParseTupleAndKeywords(args, kwargs, "i|d", evolve_kwlist, &steps, &dt_new) )
        return NULL; // exit error
    
    
    if (dt_new > 0) /*It means user wants to change dt*/
    {
        dt = dt_new;
        set_dt(dt);
    }
    // check if timesteps number is good
    if (steps <= 0) {printf("Wrong number of steps to elvove!\n"); return NULL;}
    
    printf("# Making %d steps with dt %lf\n",steps,dt);
    evolve_system(steps);
    
    Py_RETURN_NONE;
}

/*
 * Cleanup memory
 */
static PyObject* end_simulation(PyObject* self)
{
    free_mem();
    
    printf("# ENDING MOLECULAR DYNAMICS SIMULATION\n");
    
    Py_RETURN_NONE;
}