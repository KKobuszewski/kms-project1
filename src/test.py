from __future__ import print_function
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from mayavi import mlab

import MolecDyn as md # home-made module for molecular dynamics



print('# starting test of MolecDyn')

md.init_simulation(a=0.38,
                   nx=5,
                   ny=5,
                   nz=5,
                   T0=100.,
                   m=39.948,
                   f=10000.0,
                   epsilon=1.)

x,y,z = md.set_particles()

# show in mayavi
mlab.points3d(x, y, z)
mlab.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax.set_xlim([-2.,2])
ax.set_ylim([-2.,2])
ax.set_zlim([-2.,2])

ax.scatter(x, y, z, c='grey', marker='o')

plt.savefig('initial_positions.pdf')

data = np.loadtxt('coordinates.txt',skiprows=1)
x_to_compare=data[:,0]
y_to_compare=data[:,1]
z_to_compare=data[:,2]

print('comparision x')
print(np.abs(x - x_to_compare))
print()
print('comparision y')
print(np.abs(y - y_to_compare))
print()
print('comparision z')
print(np.abs(z - z_to_compare))
print()

md.end()