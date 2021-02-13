/* C++ program for estimation of Pi using MonteCarlo Simulation */
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <mpi.h>
// Defines precision for x and y values. More the interval, more the number of significant digits 
#define INTERVAL 1000
#define ANALYZE_INTERVAL 10*INTERVAL 
using namespace std;

void monteCarlo(long int &circle_points){
	double rand_x, rand_y, origin_dist;
// Randomly generated x and y values 
	rand_x = double(rand() % (INTERVAL + 1)) / INTERVAL;
	rand_y = double(rand() % (INTERVAL + 1)) / INTERVAL;
// Distance between point from the origin 
        origin_dist = rand_x * rand_x + rand_y * rand_y;
//Checking if the point is in the cu=ircle
        if (origin_dist <= 1)
            circle_points++;
}

void analyzeVariance(double pi){
	int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	double *piBuffer =NULL;
	if(world_rank ==0)
	{	
		piBuffer= new double[world_size];
	}
	MPI_Gather(&pi,1,MPI_DOUBLE,piBuffer,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	return;
}

int main()
{
	MPI_Init(NULL, NULL);
  	int world_size;
  	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  	int world_rank;
  	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	char processor_name[MPI_MAX_PROCESSOR_NAME];
  	int name_len;
  	MPI_Get_processor_name(processor_name, &name_len);
  	printf("Processor %s, rank %d\n",processor_name, world_rank);
	double pi;
    	long int circle_points = 0;

    	srand(time(NULL)); // initialize random seed

    // Total Random numbers generated = possible x values * possible y values 
    	for (long int i = 0; i < (INTERVAL * INTERVAL); i++) {

		monteCarlo(circle_points);	
        	pi = double(4 * circle_points) / (i+1);// estimated pi after this iteration

        	if (i%ANALYZE_INTERVAL ==0) {
            		analyzeVariance(pi);
			cout <<"circlepoints: "<< circle_points <<" sqrpoints:" << i+1 << " - " << pi << endl << endl;
        	}
    	}	

    	cout << "\nFinal Estimation of Pi = " << pi; // Final Estimated Value
	MPI_Finalize();
	return 0;
}
