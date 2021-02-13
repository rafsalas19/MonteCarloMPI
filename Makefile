CPPFLAGS=-std=c++11 -fopenmp 

all:MonteCarlo

MonteCarlo: main.cpp
	/opt/intel/compilers_and_libraries_2018.5.274/linux/mpi/intel64/bin/mpicxx $(CPPFLAGS) -o MonteCarlo main.cpp

clean:
	rm -f MonteCarlo
