import numpy as np

import mayavi
from mayavi import mlab

import MolecDyn as md # home-made module for molecular dynamics

nx=8
ny=8
nz=8
a=0.38

md.init_simulation(nx=nx,
                   ny=ny,
                   nz=nz,
                   a=0.38,
                   R=0.38,
                   T0=110,
                   m=39.948,
                   f=10000.0,
                   L=1.23*(nx-1)*a,
                   epsilon=1.,
                   dt=1e-03)

x,y,z,px,py,pz = md.set_particles()

#mlab.figure(1, fgcolor=(1, 1, 1), bgcolor=(0, 0, 0))
#mlab.mesh(x,y,z,transparent=True)
#mlab.points3d(0,0,0)


#r = 2.3
#pi = np.pi
#cos = np.cos
#sin = np.sin
#phi, theta = np.mgrid[0:pi:101j, 0:2 * pi:101j]

#x = r * sin(phi) * cos(theta)
#y = r * sin(phi) * sin(theta)
#z = r * cos(phi)

#md.evolve(10000)





@mlab.animate(delay=10)
def anim(x,y,z):
    f = mlab.gcf()
    plt = mlab.points3d(x,y,z, colormap="copper", scale_factor=.25)
    while(True):
        # TODO: Paralelize
        md.evolve(25)
        x,y,z = md.get_positions()
        stats = md.system_stats()
        print('Temp',stats[-1])
        plt.mlab_source.set(x=x, y=y, z=z)
        f.scene.render()
        yield

anim(x,y,z)
mlab.show()