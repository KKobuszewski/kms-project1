
#ifndef __MOLEC_DYN_H__
#define __MOLEC_DYN_H__


typedef enum {X, Y, Z} coord_t;

void init_csimulation(  const double a,
                        const double m,
                        const double T0,
                        const double epsilon,
                        const double f,
                        const double L,
                        const double dt,
                        const uint32_t nx,
                        const uint32_t ny,
                        const uint32_t nz,
                        double* base_vec1,
                        double* base_vec2,
                        double* base_vec3
                    );

void set_dt(const double dt);

void evolve_system(uint32_t steps);

double** return_positions();
double** return_momenta();
double** return_forces();

typedef enum {T_ENERGY,LJ_POTENTIAL, SP_POTENTIAL, PRESSURE, INST_TEMP} stats_type_t; // leave temperature the last!
double* get_statistics();

void free_mem();


// ============ OpenGL wrapper =======================================

void init_gl( int argc, char * argv[] );


#endif