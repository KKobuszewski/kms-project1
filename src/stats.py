from __future__ import print_function
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

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
dt= 1e-04

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
                   dt=1e-04)

x,y,z,px,py,pz = md.set_particles()






steps_to_be_done = int(1./dt)
snapshots = 100
steps_per_snapshot = steps_to_be_done/snapshots

Ek = np.zeros(snapshots+1)
V  = np.zeros(snapshots+1)
Vs = np.zeros(snapshots+1)
T  = np.zeros(snapshots+1)
P  = np.zeros(snapshots+1)

t = np.arange(float(snapshots+1))
t *= dt*steps_per_snapshot
t_max = np.max(t)
print(t)


stats = md.system_stats()
Ek[0] = stats[0]
V[0] = stats[1]
Vs[0] = stats[2]
P[0] = stats[3]
T[0] = stats[4]

for s in xrange(snapshots):
    s += 1
    
    md.evolve(steps_per_snapshot)
    
    stats = md.system_stats()
    Ek[s] = stats[0]
    V[s] = stats[1]
    Vs[s] = stats[2]
    P[s] = stats[3]
    T[s] = stats[4]


fig, (ax1, ax2, ax3)  = plt.subplots(3, 1, sharex=True)

ax1.set_title('Energy')
#ax1.set_xlabel('t')
ax1.set_ylabel('E')
ax1.set_xlim(0, t_max)
#ax1.set_ylim(-2, 2)
#ax1.set_aspect('equal', 'datalim')
ax1.plot(t,Ek,label=r'Ek')
ax1.plot(t,V ,label=r'Vpot')
#ax1.plot(t,Vs ,label=r'Vs')
ax1.plot(t,Ek + V + Vs, label=r'Etot')
ax1.legend()

ax2.set_title('Temperature')
#ax2.set_xlabel('t')
ax2.set_ylabel('T')
ax1.set_xlim(0, t_max)
#ax2.set_ylim(0, 800)
ax2.plot(t,T)

ax3.set_title('Pressure')
ax3.set_xlabel('t [ps]')
ax3.set_ylabel('P')
ax3.plot(t,P)
ax1.set_xlim(0, t_max)
#ax3.set_ylim(0, 800)

plt.show()
plt.savefig('stats_test.pdf')

