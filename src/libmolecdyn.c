#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <math.h>

typedef struct Global_t
{
    double a,
    double m,
    double T0,
    double f,
    double epsilon,
    uint32_t n,
}

Global_t global;

// TODO: not in a struct because of SIMD alignment

typedef struct Vec
{
  double x,y,z;
} Vec;

// global variables
double a, m, T0, f ;
uint32_t n;

Vec *r, *p;




inline double LennardJones_potential(Vec ri, Vec rj, const double a)
{
  const double epsilon = 1.;
  const double rij_sq = (ri.x - rj.x)*(ri.x - rj.x) +
                        (ri.y - rj.y)*(ri.y - rj.y) +
			(ri.z - rj.z)*(ri.z - rj.z);
  
  return epsilon*( pow(a*a/rij_sq,6) - 2*pow(a*a/rij_sq,3) );
}

inline double springiness_potential(Vec ri, const double f)
{
  const double L = 2.3;
  const double r = sqrt( ri.x*ri.x + ri.y*ri.y + ri.z*ri.z );
  if (r < L) 
    return 0.;
  else 
    return 0.5*f*(r - L);
}

inline double potential_energy()
{
  double V = 0;
  for (uint32_t ii = 0; ii< n*n*n; ii++)
  {
    for (uint32_t jj = 0; jj < ii; jj++)
    {
      V += LennardJones_potential(r[ii],r[jj],a);
    }
    V += springiness_potential(r[ii],f);
  }
  return V;
}