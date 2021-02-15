/* C++ program for estimation of Pi using MonteCarlo Simulation */
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <mpi.h>
#include <iomanip>
// Defines precision for x and y values. More the interval, more the number of significant digits 
#define INTERVAL 10000
#define ANALYZE_INTERVAL 100*INTERVAL 
using namespace std;

void monteCarlo(long int &circlePoints){
	double rand_x, rand_y, origin_dist;
// Randomly generated x and y values 
	rand_x = double(rand() % (INTERVAL + 1)) / INTERVAL;
	rand_y = double(rand() % (INTERVAL + 1)) / INTERVAL;
// Distance between point from the origin 
        origin_dist = rand_x * rand_x + rand_y * rand_y;
//Checking if the point is in the cu=ircle
        if (origin_dist <= 1){
        	circlePoints++;
	}
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
    	srand(time(NULL)+world_rank); // initialize random seed

    // Total Random numbers generated = possible x values * possible y values 
    	for (long int i = 0; i < (INTERVAL * INTERVAL); i++) {

		monteCarlo(circle_points);	
        	pi = double(4 * circle_points) / (i+1);// estimated pi after this iteration
        	if (i%ANALYZE_INTERVAL ==0) {
            		analyzeVariance(pi);
			//cout <<"circlepoints: "<< circle_points <<" total points:" << i+1 << " - " << pi << endl << endl;
        	}
    	}	
    	cout <<"MPI Rank: "<<world_rank<< " estimation of Pi = " << pi<<endl; // Final Estimated Value

	long int pointPair[2]= {circle_points,INTERVAL * INTERVAL};
	long int *pointPairs=NULL;
	if(world_rank ==0){
		pointPairs = new long int[world_size*2];
	}
	MPI_Gather(pointPair,2,MPI_LONG,pointPairs,2,MPI_LONG,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank==0){
		long int circPointTotal=0;
		long int pointsTotal=0;
		for(int i =0;i< world_size;++i){
			circPointTotal= pointPairs[i*2] + circPointTotal;
			pointsTotal=pointPairs[i*2+1] + pointsTotal;
		}
	        pi=double(4*circPointTotal)/pointsTotal;
        	cout<< setprecision (9)<<"Aggregated estimation: "<<pi<<endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
