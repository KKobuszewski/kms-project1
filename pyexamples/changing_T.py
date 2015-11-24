from __future__ import print_function
import math
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib
import matplotlib.pyplot as plt

import MolecDyn as md # home-made module for molecular dynamics


matplotlib.rcParams['figure.figsize'] = (16.0, 12.0)

nx= 5
ny= 5
nz= 5
R = 0.38
a= 0.37317
T0= 0.0
m= 39.948
epsilon=1.0
f= 10000.0
L= 2.3
dt = 1e-03

# GIVE TIME OF EVOLUTION IN PS AND NUMBER OF POINTS
time=15
snapshots=int(round(time/dt))

steps_to_be_done = int(round(time/dt))
steps_per_snapshot = steps_to_be_done/snapshots

md.init_simulation(nx=nx,
                   ny=ny,
                   nz=nz,
                   a=0.37317,
                   R=R,
                   T0=T0,
                   m=m,
                   f=f,
                   L=L,
                   epsilon=epsilon,
                   dt=dt)

md.set_particles()

Ts = np.logspace(0.,4.,num=10,endpoint=False)

for T in Ts:
    print(T)
    md.change_T(T)
    md.termalize()
    md.simulate(1.,100,10)
    
md.end()