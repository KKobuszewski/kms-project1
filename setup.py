from distutils.core import setup, Extension
import numpy.distutils.misc_util as np_misc_utils
import sys

#print()
#print('Installing extension for: Python',sys.version_info)
#print()

# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
import distutils.sysconfig
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")
# ======================================================================================================


# Define compilation command
module = Extension('MolecDyn',
                    sources = ['pyext/python_interface.c'],#,'libmolecdyn.c','gl_molecdyn.cpp'],
                    include_dirs=np_misc_utils.get_numpy_include_dirs(),
                    define_macros=[('NPY_NO_DEPRECATED_API','NPY_1_7_API_VERSION')],#,('VERBOSE','')],
                    undef_macros=[],
                    library_dirs=['./lib','/usr/X11R6/lib'],
                    runtime_library_dirs=['/home/konrad/CProjects/kms-project1/lib','/usr/X11R6/lib'],
                    libraries=['m','gomp', 'GL', 'GLU','glut','Xi','Xmu','molecdyn'])
"""extra_link_args=['-framework', 'OpenGL'],"""
module.extra_compile_args.extend(['--std=c99','-fopenmp'])
module.include_dirs.extend(['./src','./include'])
# ======================================================================================================


# Specify python version
if sys.version_info[0] == 3:
    module.define_macros.extend(['PY3'])
# ======================================================================================================


# Compile and install extension
setup (name = 'Molecular Dynamics package',
       version = '0.1',
       description = 'This is a python binding for a molecular dynamics C Library',
       author = 'Konrad Kobuszewski',
       ext_modules = [module])