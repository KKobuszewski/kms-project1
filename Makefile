 #------------------------------------------------------------------------------

# c compiler
CC 	  = gcc
CC_FLAGS  = -std=c99 -Wall -Wundef -m64 -march=native -O3 -msse4 -ftree-vectorizer-verbose=1 -ffast-math -fopenmp -fPIC
CC_INC    = -I./include -I./src
CC_DEFS   = 
CC_LIBS   = -lgomp -lpthread -lm

# c++ compiler
CXX	  = g++
CXX_FLAGS = -Wall -Wall -Wundef -m64 -march=native -O3 -msse4 -ftree-vectorizer-verbose=1 -ffast-math -fopenmp -fPIC
CXX_INC   = -I./include -I./src
CXX_DEFS  = 
CXX_LIBS  = -lGL -L/usr/X11R6/lib -lGLU -lglut -lXi -lXmu

# linker
LD        = g++
LD_FLAGS  = -Wl,-R/home/konrad/CProjects/kms-project1/lib -Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-soname,libmolecdyn.so -lc

#------------------------------------------------------------------------------

OBJS      = libmolecdyn.o gl_molecdyn.o 
LIB       = ./lib/libmolecdyn.so

all: $(LIB)



$(LIB): $(OBJS)
	$(LD) --shared -o $@  $(OBJS) $(LD_FLAGS) $(CXX_LIBS) $(CC_LIBS)
	mv libmolecdyn.o ./lib/libmolecdyn.o
#	mv gl_molecdyn.o ./lib/gl_molecdyn.o
# 	shell export LD_LIBRARY_PATH ... - another option to do that

.SUFFIXES:

%.o:	src/%.c
	$(CC) -o $@ -c $< $(CC_FLAGS) $(CC_INC) $(CC_DEFS)

%.o:	src/%.cpp
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(CXX_INC) $(CXX_DEFS)

clean:

	rm -f $(LIB) *.o