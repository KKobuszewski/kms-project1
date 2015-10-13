from distutils.core import setup, Extension
import numpy.distutils.misc_util as np_misc_utils

import sys

print(sys.version_info)

module = Extension('MolecDyn',
                    sources = ['python_interface.c'],
                    include_dirs=np_misc_utils.get_numpy_include_dirs(),
                    define_macros=[('NPY_NO_DEPRECATED_API','NPY_1_7_API_VERSION')],
                    undef_macros=[],
                    library_dirs=[],
                    libraries=[]
                    )

module.extra_compile_args.extend(['--std=c99'])
if sys.version_info[0] == 3:
    module.define_macros.extend(['PYTHON3'])
#else:
    #module.define_macros.extend(['_PYTHON2'])

setup (name = 'Molecular Dynamics package',
       version = '1.0',
       description = 'This is a python binding for a molecular dynamics C Library',
       ext_modules = [module])