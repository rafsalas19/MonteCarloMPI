CPPFLAGS=-std=c++11 -fopenmp 

all:MonteCarlo

MonteCarlo: main.cpp
	mpicxx $(CPPFLAGS) -o MonteCarlo main.cpp

clean:
	rm -f MonteCarlo
