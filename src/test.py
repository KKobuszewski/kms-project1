from __future__ import print_function
import MolecDyn as md

print()

md.init(a=0.38,
            n=5,
            T0=100.,
            m=39.948,
            f=10000.0)

x,y,z = md.set_particles()

print(x)
print(y)
print(z)