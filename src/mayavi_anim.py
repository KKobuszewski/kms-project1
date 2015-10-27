import numpy as np

import mayavi
from mayavi import mlab

import MolecDyn as md # home-made module for molecular dynamics

md.init_simulation(nx=5,
                   ny=5,
                   nz=5,
                   a=0.38,
                   T0=100.,
                   m=39.948,
                   f=10000.0,
                   L=2.3,
                   epsilon=1.,
                   dt=1e-03)

x,y,z,px,py,pz = md.set_particles()

#mlab.figure(1, fgcolor=(1, 1, 1), bgcolor=(0, 0, 0))
#mlab.mesh(x,y,z,transparent=True)
#mlab.points3d(0,0,0)
plt = mlab.points3d(x,y,z)

#r = 2.3
#pi = np.pi
#cos = np.cos
#sin = np.sin
#phi, theta = np.mgrid[0:pi:101j, 0:2 * pi:101j]

#x = r * sin(phi) * cos(theta)
#y = r * sin(phi) * sin(theta)
#z = r * cos(phi)



@mlab.animate(delay=15)
def anim():
    f = mlab.gcf()
    #for ii in xrange(50):
    while(True):
        # TODO: Paralelize
        md.evolve(1000)
        x,y,z = md.get_positions()
        print(x)
        plt.mlab_source.set(x=x, y=y, z=z)
        f.scene.render()
        yield

anim()
mlab.show()