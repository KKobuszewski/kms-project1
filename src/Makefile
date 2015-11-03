 #------------------------------------------------------------------------------

# c compiler
CC 	  = gcc
CC_FLAGS  = -std=c99 -Wall -Wundef -m64 -march=native -O3 -msse4 -ftree-vectorizer-verbose=1 -fopenmp -fPIC
CC_INC    = -I.
CC_DEFS   = 
CC_LIBS   = -lgomp -lpthread -lm

# c++ compiler
CXX	  = g++
CXX_FLAGS = -Wall -Wundef -m64 -march=native -fPIC
CXX_INC   = -I.
CXX_DEFS  = 
CXX_LIBS  = -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lXi -lXmu

# linker
LD        = g++
LD_FLAGS  = 

#------------------------------------------------------------------------------

OBJS      = libmolecdyn.o gl_molecdyn.o 
LIB       = ../lib/libmolecdyn.so

all: $(LIB)



$(LIB): $(OBJS)
	$(LD) --shared -o $@  $(OBJS) $(LD_FLAGS) $(CXX_LIBS) $(CC_LIBS)
# 	shell export LD_LIBRARY_PATH ... - another option to do that

.SUFFIXES:

%.o:	%.c
	$(CC) -o $@ -c $< $(CC_FLAGS) $(CC_INC) $(CC_DEFS)

%.o:	%.cpp
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(CXX_INC) $(CXX_DEFS)

clean:

	rm -f $(LIB) *.o