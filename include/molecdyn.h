
#ifndef __MOLEC_DYN_H__
#define __MOLEC_DYN_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Global_t
{
    /* Constants */
    double a;
    double R;
    double m;
    double T0;
    double epsilon;
    double f;
    double L;
    double lm;
    double kb;
    double dt;
    uint32_t nx;
    uint32_t ny;
    uint32_t nz;
    uint32_t N;
    
    /* Base vectors of lattice*/
    double base_vec1[3];
    double base_vec2[3];
    double base_vec3[3];
    
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

extern Global_t global;

typedef enum {X, Y, Z} coord_t;
typedef enum {UNIFORM, BOLTZMANN} distribution_t;

void init_csimulation(  const double a,
                        const double R,
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
void reset_lattice();
void change_lattice(double* base_vec1, double* base_vec2, double* base_vec3);
void change_a(double a, double* base_vec1, double* base_vec2, double* base_vec3);
void change_R(const double R);
void change_T(const double T0);
void set_dt(const double dt);

void termalize_system();
void evolve_system(uint32_t steps);
void perform_experiment(double time, uint16_t steps_per_stats, uint16_t steps_per_positions);

double** return_positions();
double** return_momenta();
double** return_forces();

typedef enum {T_ENERGY,LJ_POTENTIAL, SP_POTENTIAL, PRESSURE, INST_TEMP} stats_type_t; // leave temperature the last!
double* get_statistics();

void free_mem();


// ============ OpenGL wrapper =======================================


void init_gl( int argc, char * argv[] );

#ifdef __cplusplus
};
#endif

#endif