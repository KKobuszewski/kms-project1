File list:
    libmolecdyn.so                           - C/C++ library
    MolecDyn.so                              - Python module
    Molecular_Dynamics_package-1.0.egg-info  - Python package info


Right compilation output:
    rm: nie można usunąć „/usr/local/lib/python2.7/dist-packages/Molec*”: Nie ma takiego pliku ani katalogu
    rm -f ../lib/libmolecdyn.so *.o
    gcc -o libmolecdyn.o -c libmolecdyn.c -std=c99 -Wall -Wundef -m64 -march=native -O3 -msse4 -ftree-vectorizer-verbose=1 -fopenmp -fPIC -I. 
    g++ -o gl_molecdyn.o -c gl_molecdyn.cpp -Wall -Wundef -m64 -march=native -fPIC -I. 
    g++ --shared -o ../lib/libmolecdyn.so  libmolecdyn.o gl_molecdyn.o   -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lXi -lXmu -lgomp -lpthread -lm
    Installing extension for: python2.7 --version
    running build
    running build_ext
    building 'MolecDyn' extension
    C compiler: x86_64-linux-gnu-gcc -pthread -DNDEBUG -g -fwrapv -O2 -Wall -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -g -fstack-protector-strong -Wformat -Werror=format-security -fPIC

    creating build
    creating build/temp.linux-x86_64-2.7
    compile options: '-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION -I/home/konrad/.local/lib/python2.7/site-packages/numpy/core/include -I/usr/include/python2.7 -c'
    extra options: '--std=c99 -fopenmp'
    x86_64-linux-gnu-gcc: libmolecdyn.c
    x86_64-linux-gnu-gcc: python_interface.c
    python_interface.c: In function ‘parse_args’:
    python_interface.c:132:5: warning: implicit declaration of function ‘strdup’ [-Wimplicit-function-declaration]
        animation_str   = strdup ("no");
        ^
    python_interface.c:132:21: warning: assignment makes pointer from integer without a cast
        animation_str   = strdup ("no");
                        ^
    python_interface.c:133:21: warning: assignment makes pointer from integer without a cast
        vel_distrib_str = strdup ("no");
                        ^
    python_interface.c:134:21: warning: assignment makes pointer from integer without a cast
        gravitation_str = strdup ("no");
                        ^
    python_interface.c: In function ‘set_particles’:
    python_interface.c:260:13: warning: assignment makes pointer from integer without a cast
        argv [0]=strdup ("test.py");
                ^
    x86_64-linux-gnu-gcc: gl_molecdyn.cpp
    cc1plus: warning: command line option ‘-std=c99’ is valid for C/ObjC but not for C++
    creating build/lib.linux-x86_64-2.7
    c++ -pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -Wl,-z,relro -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -D_FORTIFY_SOURCE=2 -g -fstack-protector-strong -Wformat -Werror=format-security -Wl,-Bsymbolic-functions -Wl,-z,relro -D_FORTIFY_SOURCE=2 -g -fstack-protector-strong -Wformat -Werror=format-security build/temp.linux-x86_64-2.7/python_interface.o build/temp.linux-x86_64-2.7/libmolecdyn.o build/temp.linux-x86_64-2.7/gl_molecdyn.o -L../lib -L/usr/X11R6/lib -Wl,-R/home/konrad/CProjects/kms-project1/lib -Wl,-R/usr/X11R6/lib -lm -lgomp -lGL -lGLU -lglut -lXi -lXmu -lmolecdyn -o build/lib.linux-x86_64-2.7/MolecDyn.so
    running install
    running build
    running build_ext
    running install_lib
    copying build/lib.linux-x86_64-2.7/MolecDyn.so -> /usr/local/lib/python2.7/dist-packages
    running install_egg_info
    Writing /usr/local/lib/python2.7/dist-packages/Molecular_Dynamics_package-0.1.egg-info

