#!/bin/bash

set -x


#module load mpi/hpcx
source /opt/hpcx-v2.13-gcc-MLNX_OFED_LINUX-5-redhat8-cuda11-gdrcopy2-nccl2.12-x86_64/hpcx-init.sh 
hpcx_load





export PIN_PROCESSOR_LIST="--bind-to cpulist:ordered --cpu-set 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119 --report-bindings "

export OMPI_MCA_coll=^hcoll

echo localhost > hostfile.txt
export AMD_PERF_DIR=/home/rafsalas/AMDuProf_Linux_x64_4.0.341/bin
PROF_CMD="$AMD_PERF_DIR/AMDuProfCLI collect --config tbp --mpi --output-dir /home/rafsalas/MonteCarloMPI/profout"

mpirun --allow-run-as-root -machinefile ./hostfile.txt -np 120 $PIN_PROCESSOR_LIST --rank-by slot -mca coll ^hcoll -x LD_LIBRARY_PATH -x PATH -x PWD $PROF_CMD ./MonteCarlo

