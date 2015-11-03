from distutils.core import setup, Extension
import numpy.distutils.misc_util as np_misc_utils
import sys

print(sys.version_info)

# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
import distutils.sysconfig
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")
# ==================================

module = Extension('MolecDyn',
                    sources = ['python_interface.c'],#'libmolecdyn.c'],
                    include_dirs=np_misc_utils.get_numpy_include_dirs(),
                    define_macros=[('NPY_NO_DEPRECATED_API','NPY_1_7_API_VERSION')],#,('VERBOSE','')],
                    undef_macros=[],
                    library_dirs=["."],
                    runtime_library_dirs=["/home/konrad/CProjects/kms-project1/lib"],
                    libraries=['m','gomp','molecdyn'])

module.extra_compile_args.extend(['--std=c99','-fopenmp'])
if sys.version_info[0] == 3:
    module.define_macros.extend(['PYTHON3'])
#else:
    #module.define_macros.extend(['_PYTHON2'])

setup (name = 'Molecular Dynamics package',
       version = '1.0',
       description = 'This is a python binding for a molecular dynamics C Library',
       ext_modules = [module])