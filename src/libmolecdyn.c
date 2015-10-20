#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <math.h>

#include "molecdyn.h"

#define M_PI 3.141592653589793

typedef struct Global_t
{
    double a;
    double m;
    double T0;
    double epsilon;
    double f;
    double L;
    double lm;
    double kb;
    uint32_t nx;
    uint32_t ny;
    uint32_t nz;
    uint32_t N;
    
    /* Arrays with data */
    double* x_arr;
    double* y_arr;
    double* z_arr;
    double* px_arr;
    double* py_arr;
    double* pz_arr;
    double* fx_arr;
    double* fy_arr;
    double* fz_arr;
} Global_t;

Global_t global;

double* wrk_arr1;
double* wrk_arr2;

// TODO: Move static inline functions to header in case to make them inline in other compilation units.
//       OR MOVE GLOBAL STRUCTRE TO MAKE IT VISIBLE IN OTHER COMPILATION UNITS

// ============== Set lattice methods ============================================ /

/*
 * This function set given coordinate of position vector for every particle.
 * param coord_arr - array of single coordinates of particle
 * param bi_coord - given coordinate of i-th base vector
 */
inline void vecCoordxbaseCoord(double* coord_arr, const double b1_coord, const double b2_coord, const double b3_coord)
{
    // TODO: Check if really quicker with register variables
    // TODO: Make use of vectorization and SIMD alignmnet!
    uint32_t ixyz = 0;
    const uint32_t nx = global.nx;
    const uint32_t ny = global.ny;
    const uint32_t nz = global.nz;
    //double register const n_dlb = (double)global.n;
    // TODO: Unroll this loop!
    // TODO: OpenMP
    for (uint32_t ix = 0; ix < nx; ix++)
    {
        for (uint32_t iy = 0; iy < ny; iy++)
        {
            for (uint32_t iz = 0; iz < nz; iz++)
            {
                //ixyz = ix + iy*nx + iz*ny*nx;
                coord_arr[ixyz] = (ix - (nx-1)/2.)*b1_coord + (iy - (ny-1)/2.)*b2_coord + (iz - (nz-1)/2.)*b3_coord;
                ixyz += 1;
            }
        }
    }
    
    // idea for unrolling:
//     uint32_t ix=0, iy=0, iz=0
//     for (uint32_t ii = 0; ii < nx*ny*nz; ii++)
//     {
//         coord_arr[ii] = (ix - (n-1)/2.)*b1_coord + (iy - (n-1)/2.)*b2_coord + (iz - (n-1)/2.)*b3_coord;
//         ix = (ix + 1)%nx;
//         iy = (iy + 1)%ny;
//         iz = (iz + 1)%nz;
//     }
}


/*
 * This function sets particles in a lattice generated by basis vectors base_veci.
 */
inline void set_lattice(double* base_vec1,
                        double* base_vec2,
                        double* base_vec3)
{
    // TODO: Make use of OpenMP and omp_set_nested!
    vecCoordxbaseCoord(global.x_arr, base_vec1[X], base_vec2[X], base_vec3[X]);
    vecCoordxbaseCoord(global.y_arr, base_vec1[Y], base_vec2[Y], base_vec3[Y]);
    vecCoordxbaseCoord(global.z_arr, base_vec1[Z], base_vec2[Z], base_vec3[Z]);
}

// ============= Draw momentum methods =================================================== /

typedef enum {UNIFORM, BOLTZMANN} distribution_t;

inline void drawMomVecCoordUniform(double* coord_arr)
{
    register const uint32_t N = global.nx*global.ny*global.nz;
    register const double lm = global.lm;
    
    for (uint32_t ii = 0; ii < N; ii++)
    {
        coord_arr[ii] = lm*(2*((double)rand()/((double)RAND_MAX)) - 1);
    }
}

// TODO: Better random number generator
inline void drawMomVecCoordBoltzmann(double* coord_arr)
{
    register const uint32_t N = global.nx*global.ny*global.nz;
    register const double m = global.m;
    register const double kb = global.kb;
    register const double T0 = global.T0;
        
    for (register uint32_t ii = 0; ii < N; ii++)
    {
        // TODO: Check if float wouldn`t be precise enough
        register double X = ((double)rand() + 1.)/((double)RAND_MAX + 1.); // random number in (0,1]
        register double sign = (1. - 2.*(rand()%2)); // random sign
        coord_arr[ii] = sign * sqrt(m*kb*T0 * ((-1.)*log(X))); // \pm \sqrt{2 m E_k}
    }
}


// draw momentum from chosen distribution
inline void drawMomVecCoord(double* coord_arr, distribution_t distribution)
{
    if      (distribution == BOLTZMANN) drawMomVecCoordBoltzmann(coord_arr);
    else if (distribution == UNIFORM  ) drawMomVecCoordUniform(coord_arr);
}

inline void set_mean_momentum_zero()
{
    register const uint32_t N = global.nx*global.ny*global.nz;
    
    double mean_momentum[3];
    mean_momentum[X] = 0.;
    mean_momentum[Y] = 0.;
    mean_momentum[Z] = 0.;
    
    // TODO: reduction with OpenMP
    for (register uint32_t ii=0; ii<N; ii++)
    {
        mean_momentum[X] += global.px_arr[ii];
    }
    for (register uint32_t ii=0; ii<N; ii++)
    {
        mean_momentum[Y] += global.py_arr[ii];
    }
    for (register uint32_t ii=0; ii<N; ii++)
    {
        mean_momentum[Z] += global.pz_arr[ii];
    }
    
    mean_momentum[X] /= (double)N;
    mean_momentum[Y] /= (double)N;
    mean_momentum[Z] /= (double)N;
    
    // TODO: Check which is quicker
    
    for (register uint32_t ii=0; ii<N; ii++)
    {
        global.px_arr[ii] -= mean_momentum[X];
        global.py_arr[ii] -= mean_momentum[Y];
        global.pz_arr[ii] -= mean_momentum[Z];
    }
    
    printf("# MEAN MOMENTUM AFTER DRAWING\n");
    printf("(%5.5lf, %5.5lf, %5.5lf)\n\n",mean_momentum[X],mean_momentum[Y],mean_momentum[Z]);
}

// TODO: Add different distributions. MAXWELL-BOLTZMANN!
inline void set_momenta()
{
    drawMomVecCoord(global.px_arr, BOLTZMANN);
    drawMomVecCoord(global.py_arr, BOLTZMANN);
    drawMomVecCoord(global.pz_arr, BOLTZMANN);
        
    set_mean_momentum_zero();
}

// ============= Count forces ============================================================== /

// http://chemwiki.ucdavis.edu/Physical_Chemistry/Physical_Properties_of_Matter/Intermolecular_Forces/Lennard-Jones_Potential


// ============= Count statistics ========================================================= /

inline double kinetic_energy()
{
    const uint32_t N = global.nx*global.ny*global.nz;
    double T = 0.;
    
    // TODO: Optimization via 3 worker arrays? and 3 OpenMP Sections
    
    // Reduction with OpenMP
    for (uint32_t ii = 0; ii < N; ii++)
    {
        T += global.px_arr[ii]*global.px_arr[ii] + global.py_arr[ii]*global.py_arr[ii] + global.pz_arr[ii]*global.pz_arr[ii];
    }
    
    return 0.5*T/global.m;
}

inline double springiness_potential()
{
    const uint32_t N = global.nx*global.ny*global.nz;
    
    // TODO: Place this with force counting and make sure potential will be counted after
    // TODO: Check if using wrk_arr is quicker
    // count distances from (0,0,0)
    for (uint32_t ii = 0; ii < N; ii++)
    {
        wrk_arr1[ii] = global.x_arr[ii]*global.x_arr[ii] + global.y_arr[ii]*global.y_arr[ii] + global.z_arr[ii]*global.z_arr[ii];
    }
    /*
     * NOTE: |r_i| in wrk_arr1
     */
    
    
    double V_sp = 0.;
    const double L = global.L;
    
    // omp redution with condition?
    for (uint32_t ii = 0; ii < N; ii++)
    {
        if (wrk_arr1[ii] >= L)
        {
            V_sp += (wrk_arr1[ii] - L)*(wrk_arr1[ii] - L);
        }
    }
    
    return 0.5*global.f*V_sp;
}


inline double pressure()
{
    const uint32_t N = global.nx*global.ny*global.nz;
    const double L = global.L;
    const double four_pi_L_sq = 4.*M_PI*global.L*global.L;
    double P =0.;
    
    
    for (uint32_t ii = 0; ii < N; ii++)
    {
        if (wrk_arr1[ii] >= L)
        {
            P += (wrk_arr1[ii] - L);
        }
    }
    
    return P/four_pi_L_sq;
}


inline double LJ_potential()
{
    const uint32_t N = global.nx*global.ny*global.nz;
    const double a_sq = global.a*global.a;
    const double epsilon = global.epsilon;
    double rij_sq;
    double xij;
    double yij;
    double zij;
    double V_LJ = 0.;
    
    for (uint32_t ii = 0; ii < N; ii++)
    {
        for (uint32_t jj = 0; jj < ii; jj++)
        {
            //printf("ii: %u\tjj: %u",ii,jj);
            
            xij = global.x_arr[ii] - global.x_arr[jj];
            yij = global.y_arr[ii] - global.y_arr[jj];
            zij = global.z_arr[ii] - global.z_arr[jj];
            
            rij_sq = xij*xij + yij*yij + zij*zij;
            rij_sq = a_sq/rij_sq;
            
            V_LJ += ( pow(rij_sq,6) - 2.*pow(rij_sq,3) );
        }
    }
    
    return epsilon*V_LJ;
}


// ============= Functions to be called out of library ===================================== /

void init_csimulation(  const double a,
                        const double m,
                        const double T0,
                        const double epsilon,
                        const double f,
                        const double L,
                        const uint32_t nx,
                        const uint32_t ny,
                        const uint32_t nz,
                        double* base_vec1,
                        double* base_vec2,
                        double* base_vec3
                    )
{
    // Set parameters
    global.a = a;
    global.m = m;
    global.T0 = T0;
    global.epsilon = epsilon;
    global.f = f;
    global.L = L;
    global.nx = nx;
    global.ny = nz;
    global.nz = nz;
    global.N = nx*ny*nz;
    global.kb = 0.00831;
    global.lm = sqrt(3.*T0*0.00831*m); // sqrt(3.*T0*kB*m) ale przyjmujemy do celow symulacji m = 1 <- przeskalowanie przy zwracaniu wyników
    
    // TODO: Check if it wouldn't be quicker to make contigouous 3-array.
    // Alloc mem for positions' vector
    global.x_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    global.y_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    global.z_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    
    // Alloc mem for momenta's vector
    global.px_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    global.py_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    global.pz_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    
    // Alloc mem for forces' vector
    global.fx_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    global.fy_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    global.fz_arr = (double*)malloc( sizeof(double) * nx*ny*nz );
    
    // Additional worker arrays
    wrk_arr1 = (double*)malloc( sizeof(double) * nx*ny*nz );
    wrk_arr2 = (double*)malloc( sizeof(double) * nx*ny*nz );
    
    printf("# CONSTRUCTING LATTICE %ux%ux%u\n",nx,ny,nz);
    printf("lattice constant:  %lf",a);
    printf("lattice will be constucted on base vectors: \n");
    printf("(%1.3lf,%1.3lf,%1.3lf)\n",base_vec1[0],base_vec1[1],base_vec1[2]);
    printf("(%1.3lf,%1.3lf,%1.3lf)\n",base_vec2[0],base_vec2[1],base_vec2[2]);
    printf("(%1.3lf,%1.3lf,%1.3lf)\n",base_vec3[0],base_vec3[1],base_vec3[2]);
    
    // NOTE: Assuming vectors are normalized!
    for (int ii=0; ii < 3; ii++)
    {
        base_vec1[ii] *= a;
	base_vec2[ii] *= a;
	base_vec3[ii] *= a;
    }
    set_lattice(base_vec1, base_vec2, base_vec3);
    
    set_momenta();
}

// copy pointers to data
double** return_positions()
{
    double** positions = (double**) malloc(3 * sizeof(double*));
    
    positions[0] = global.x_arr;
    positions[1] = global.y_arr;
    positions[2] = global.z_arr;
    
    return positions;
}

double** return_momenta()
{
    double** momenta = (double**) malloc(3 * sizeof(double*));
    
    momenta[0] = global.px_arr;
    momenta[1] = global.py_arr;
    momenta[2] = global.pz_arr;
    
    return momenta;
}

double** return_forces()
{
    double** forces = (double**) malloc(3 * sizeof(double*));
    
    forces[0] = global.fx_arr;
    forces[1] = global.fy_arr;
    forces[2] = global.fz_arr;
    
    return forces;
}

double* get_statistics()
{
    //kinetic energy, LJ potential energy, springiness potential energy, pressure, instantaneous tempretature
//     double* stats = (double*) malloc( (INST_TEMP+1) * sizeof(double) ); // to nice extending for another constants
    double* stats = (double*) malloc( (5) * sizeof(double) ); // to nice extending for another constants
    
    stats[0] = kinetic_energy();
    printf("kinetic: %lf\n",stats[0]);
    stats[1] = LJ_potential();
    printf("Lj     : %lf\n",stats[1]);
    stats[2] = springiness_potential();
    printf("spring : %lf\n",stats[2]);
    stats[3] = pressure();
    printf("press. : %lf\n",stats[3]);
    stats[4] = global.T0;
    printf("temp.  : %lf\n",stats[4]);
    
    return stats;
}

void free_mem()
{
    
    free(global.x_arr);
    free(global.y_arr);
    free(global.z_arr);
    
    free(global.px_arr);
    free(global.py_arr);
    free(global.pz_arr);
    
    free(global.fx_arr);
    free(global.fy_arr);
    free(global.fz_arr);
    
    free(wrk_arr1);
    free(wrk_arr2);
}