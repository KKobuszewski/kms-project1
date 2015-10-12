 #------------------------------------------------------------------------------

CC=gcc
CXX=g++
CC_FLAGS= -Wall
CXX_FLAGS= -Wall


SRC= main.cpp
PROG= simulation.exe
INC= -I. 
LIB= 

#------------------------------------------------------------------------------



all: $(PROG)



$(PROG): $(SRC)

	$(CXX) $(INC) $(SRC) -o$(PROG) $(LIB)

clean:

	rm -f $(PROG)