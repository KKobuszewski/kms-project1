
#ifndef __MOLEC_DYN_H__
#define __MOLEC_DYN_H__



void init_csimulation(   const double a,
                        const double m,
                        const double T0,
                        const double f,
                        const double epsilon,
                        const uint32_t nx,
                        const uint32_t ny,
                        const uint32_t nz,
                        double* base_vec1,
                        double* base_vec2,
                        double* base_vec3
                    );

void get_data(double** x_arr_ptr, double** y_arr_ptr, double** z_arr_ptr, double** vx_arr_ptr, double** vy_arr_ptr, double** vz_arr_ptr);
double** return_positions();

#endif