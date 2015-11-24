# Molecular Dynamics library
This simple molecular dynamics library with Python bindings was written for the prupose of students' laboratory project in Computer Simulation Methods course taught during the autumn semester of Academic Year 2015/2016 on Faculty of Physics Warsaw University of Technology.
Simulation enables investigation of molecular dynamics in set of particles interacting with Lennard-Jones potential. Particles are placed in spherical volume and the interaction with medium outside bondouaries is given by $ \boldsymbol{F}^S_i = f \left( L - \boldsymbol{r}_i \right) \frac{\boldsymbol{r}_i}{\vert \boldsymbol{r}_i \vert} $.

![Molecular Dynamics in Mayavi](https://github.com/KKobuszewski/kms-project1/blob/master/mayavi_screenshot.png)

## Repository structure
Directories` contents description:
- Directory `build` is used for files created by python's setuptools.distutils when building Python wrapper.
- Directory `include` contains C headers to be included when using library in C/C++ programs. It could be done by adding to compile command `-I<path-to-repository>/include`
- Directory `lib` is the destination location of shared C library that can be linked with C/C++ program or Python extension. When using with C/C++ it could be linked to program with `-L<path-to-repository>/lib -Wl,-R<path-to-repository>/lib -lmolecdyn`
- Directory `OpenGL` contains an example of C++ program with linked libmolecdyn.so library. This is the real-time animation of results provided by simulation code. The visualiztion was written in OpenGL.
- Directory `pyexamples` contains examples of using Python wrapper to controling simulation parameters, simulation's data processing and visualization.
- Directory `pyext` contains source code of Python interface to library. Building this extension with `python setup.py build_ext -i' enables loading library as Python module: ```python import MolecDyn```
- Directory `src` contains library source code.
- Directory `test` contains Python script for testing corectness of library and Python module after compilation process.

## Installation

Source code was tested with gcc 4.9.2 and Python 2.7.9 on Ubuntu 15.04.

# Dependencies
To run all given examples there are following Python modules required:
- `numpy` 1.7 or newer
- `matplotlib` 1.4.3 or newer
- `distutils` and `setuptools`
- `Mayavi` (`sudo apt-get install mayavi2`)
Requred C++ libraries:
- OpenGL and GLUT

# Building with python extension

Change path in file `install_extension.sh` lines 3 and 10 for your Python modules dist packages folder.
Change path in file `setupy.py` in line 25 for your <path-to-repository>/lib.

run `./install_extension.sh` script


# Building C library only

run `make` in repository folder.

