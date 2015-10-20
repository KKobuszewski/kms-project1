#from __future__ import print_function
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
#from mayavi import mlab

import MolecDyn as md # home-made module for molecular dynamics

nx= 0.380000
ny= 0.000000
nz= 0.000000
a= 0.000000
T0= 100.000000
m= 39.948000
epsilon=1
f= 10000.000000
L= 1.200000

print('# starting test of MolecDyn')

md.init_simulation(nx=5,
                   ny=5,
                   nz=5,
                   a=0.38,
                   T0=100.,
                   m=39.948,
                   f=10000.0,
                   L=1.2,
                   epsilon=1.)

x,y,z,px,py,pz = md.set_particles()

plt.hist([px,py,pz],label=['px','py','pz'])
plt.title("px,py,pz histogram")
plt.xlabel("momentum")
plt.ylabel("frequency")

#plt.show()
plt.savefig('momentum_histogram.pdf')

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
#print(x)
#print(x_to_compare)
print()
print('comparision y')
print(np.max(np.abs(y - y_to_compare)))
#print(y)
#print(y_to_compare)
print()
print('comparision z')
print(np.max(np.abs(z - z_to_compare)))
#print(z)
#print(z_to_compare)
print()

print()

md.end()