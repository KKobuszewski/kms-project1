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



all: $(PROG)



$(PROG): $(SRC)

	$(CXX) $(INC) $(SRC) -o$(PROG) $(LIB)

clean:

	rm -f $(PROG)