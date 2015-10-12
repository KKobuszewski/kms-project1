
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <math.h>

using namespace std;

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


int main()
{
  ifstream infile;
  ofstream outfile;
  
  
  infile.open("input.txt");
  char buf[100];
  
  while(!infile.eof())
  {
    infile >> buf;
    //cout<<buf<<endl;
    if (!strcmp("a",buf)) infile >> a;
    if (!strcmp("n",buf)) infile >> n;
    if (!strcmp("T0",buf)) infile >> T0;
    if (!strcmp("m",buf)) infile >> m;
    if (!strcmp("f",buf)) infile >> f;
  }
  
  infile.close();
  
  cout <<"a "<<a<<endl;
  cout <<"n "<<n<<endl;
  
  // base vector of a crystal
  Vec b1, b2, b3;
  b1.x = a;
  b1.y = 0;
  b1.z = 0;
  
  b2.x = a/2.;
  b2.y = a*sqrt(3)/2.;
  b2.z = 0.;
  
  b3.x = a/2.;
  b3.y = a*sqrt(3)/6.;
  b3.z = a*sqrt(2./3.);
  
  
  // constants
  const double kb = 0.00831;
  const double lm = sqrt(3.*T0*kb*m);
  
  // allocate arrays
  r = new Vec[n*n*n];
  p = new Vec[n*n*n];
  
  // generate data
  outfile.open("coordinates.txt");
  //outfile << "ix\tiy\tiz\tx\ty\tz" << endl;
  outfile << "\tx\ty\tz\tvx\tvy\tvz" << endl;
  uint32_t ixyz;
  for (uint32_t ix = 0; ix < n; ix++)
  {
    for (uint32_t iy = 0; iy < n; iy++)
    {
      for (uint32_t iz = 0; iz < n; iz++)
      {
	ixyz = ix + iy*n + iz*n*n;
	r[ixyz].x = (ix - (n-1)/2.)*b1.x + (iy - (n-1)/2.)*b2.x + (iz - (n-1)/2.)*b3.x;
	r[ixyz].y = (ix - (n-1)/2.)*b1.y + (iy - (n-1)/2.)*b2.y + (iz - (n-1)/2.)*b3.y;
	r[ixyz].z = (ix - (n-1)/2.)*b1.z + (iy - (n-1)/2.)*b2.z + (iz - (n-1)/2.)*b3.z;
	
	p[ixyz].x = lm*(2*drand48() - 1);
	p[ixyz].y = lm*(2*drand48() - 1);
	p[ixyz].z = lm*(2*drand48() - 1);
	
	
	//outfile<<ix<<"\t"<<iy<<"\t"<<iz<<"\t"<<r[0].x<<"\t"<<r[0].y<<"\t"<<r[0].z<<endl;
	outfile<<r[ixyz].x<<"\t"<<r[ixyz].y<<"\t"<<r[ixyz].z<<endl;
	
	/*
	 * TODO: wipisac pedy i wyswietlic w gnuplocie
	 * http://stackoverflow.com/questions/12503786/gnuplot-coloring-3d-vectors
	 * 
	 * 
	 * 
	 * 
	 */
      }
    }
  }
  outfile.close();
  
  // count potential energy
  double V = potential_energy();
  
  
  cout << "potential energy: " << V << endl;
  
  return EXIT_SUCCESS;
}
