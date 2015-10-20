 #------------------------------------------------------------------------------

# c compiler
CC 	  = gcc
CC_FLAGS  = -Wall -Wundef -m64 -march=native -O3 -msse4 -ftree-vectorizer-verbose=1 -fopenmp -fPIC
CC_INC    = -I.
CC_DEFS   = 
CC_LIBS   = -lgomp -lpthread -lm

# c++ compiler
CXX	  = g++
CXX_FLAGS = -Wall -Wundef -m64 -march=native -O3 -msse4 -ftree-vectorizer-verbose=1 -fopenmp -fPIC
CXX_INC   = -I.
CXX_DEFS  = 
CXX_LIBS  = -lgomp -lpthread -lm

# linker
LD        = g++
LD_FLAGS  = 

#------------------------------------------------------------------------------

SRC       = main.cpp
PROG      = libmolecdyn.so

all: $(PROG)



$(PROG): $(SRC)

	$(CXX) -c $(SRC) $(CXX_FLAGS) $(CXX_INC) $(CXX_LIBS) $(CXX_DEFS)
	$(CXX) -

clean:

	rm -f $(PROG)