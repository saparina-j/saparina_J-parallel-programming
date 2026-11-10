#!/bin/bash
/$//
#SBATCH --job-name=matrix_mpi
/$//
#SBATCH --time=00:30:00
/$//
#SBATCH --nodes=1
/$//
#SBATCH --ntasks=1
/$//
#SBATCH --output=matrix_mpi_%j.out
/$//

/$//
module purge
/$//
module load intel/mpi4
/$//

/$//
# N и число процессов задаются при отправке через --export
/$//
N=${N:-1200}
/$//
FILE_A=A_${N}.txt
/$//
FILE_B=B_${N}.txt
/$//
FILE_C=C_${N}_${SLURM_NTASKS}.txt
/$//

/$//
mpirun -np $SLURM_NTASKS ./matrix_mpi $N $FILE_A $FILE_B $FILE_C
/$//
