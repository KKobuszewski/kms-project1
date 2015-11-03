from __future__ import print_function
import numpy as np
import math
from math import pi
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
#from mayavi import mlab

import MolecDyn as md # home-made module for molecular dynamics

nx= 5
ny= 5
nz= 5
a= 0.38
T0= 100.0
m= 39.948
epsilon=1.0
f= 10000.0
L= 2.3
dt= 2e-03
kb=8.31*1e-3

print('# starting test of MolecDyn')

md.init_simulation(nx=5,
                   ny=5,
                   nz=5,
                   a=0.38,
                   T0=100.,
                   m=39.948,
                   f=10000.0,
                   L=2.3,
                   epsilon=1.,
                   dt=2e-03)

x,y,z,px,py,pz = md.set_particles()

p = np.linspace(0.,20.,num=200)
C = math.sqrt(kb*T0/m)

plt.hist(np.sqrt(px**2+py**2+pz**2)/m,label='|p|',normed=True)
#plt.plot(p/m,4*pi*(m/(2*pi*kb*T0))**(3/2)*(p/m)**2*np.exp(-(p/m)**2/(2*m*kb*T0)))
plt.axvline(math.sqrt(2*kb*T0/m),color='red')
plt.title("velocity histogram")
plt.xlabel("velocity")
plt.ylabel("frequency")

plt.show()
plt.savefig('velocity_histogram.pdf')

print('mean momentum via Python: ({},{},{})'.format(np.mean(px),np.mean(py),np.mean(pz)))

sprinigness_pot_buff = L-np.sqrt(x**2+y**2+z**2);
stats = md.system_stats()
print('stats array shape:',stats.shape)
print('Kinetic energy   :\t',stats[0],'\t kinetic energy via Python',0.5*np.sum(px**2+py**2+pz**2)/m)
print('LJ potential     :\t',stats[1],'\t LJ pot. energy via Python')
print('spring. potential:\t',stats[2],'\t                via Python', 0.5*f*np.sum((sprinigness_pot_buff[np.where(sprinigness_pot_buff >= 0.)])**2))
print('pressure         :\t',stats[4],'\t                via Python')

# show in mayavi
#mlab.points3d(x, y, z)
#mlab.show()
#mlab.savefig('mayavi_positions.pdf')

data = np.loadtxt('coordinates.txt',skiprows=1)
x_to_compare=data[:,0]
y_to_compare=data[:,1]
z_to_compare=data[:,2]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax.set_xlim([-2.,2])
ax.set_ylim([-2.,2])
ax.set_zlim([-2.,2])

ax.scatter(x, y, z, c='grey', marker='o')
ax.scatter(x_to_compare, y_to_compare, z_to_compare, c='grey', marker='^')

#plt.show()
plt.savefig('initial_positions.pdf')



print('comparision x')
print(np.max(np.abs(x - x_to_compare)))
print()
print('comparision y')
print(np.max(np.abs(y - y_to_compare)))
print()
print('comparision z')
print(np.max(np.abs(z - z_to_compare)))
print()

print()
print('testing get positions')
x2,y2,z2 = md.get_positions()
print('x max diff.:',np.max(np.abs(x - x2)))
print('y max diff.:',np.max(np.abs(y - y2)))
print('z max diff.:',np.max(np.abs(z - z2)))
print()

#print('forces for single step')
#fx,fy,fz = md.get_forces()
#print(np.dstack([fx,fy,fz]))

print('single step')
md.evolve(1)
x2,y2,z2 = md.get_positions()
#print(np.dstack([x2,y2,z2]))
print(np.sqrt(np.max(x2**2+y2**2+z2**2)))

print('testing if evolution places')
print('x max diff.:',np.max(np.abs(x - x2)))
print('y max diff.:',np.max(np.abs(y - y2)))
print('z max diff.:',np.max(np.abs(z - z2)))



plt.clf()

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax.set_xlim([-2.,2])
ax.set_ylim([-2.,2])
ax.set_zlim([-2.,2])

#ax.scatter(x, y, z, c='grey', marker='o')
#ax.scatter(x2, y2, z2, c='red', marker='o')

#plt.show()
#plt.savefig('evolution_step.pdf')

md.end()