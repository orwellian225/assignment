#!/bin/bash
#SBATCH -p batch
#SBATCH -N 2
#SBATCH -n 8
#SBATCH --mem=256
#SBATCH --time=00:02:00
#SBATCH -J PC-Assigment-Scan-24262825
#SBATCH -o /home-mscluster/bgriffiths/scan/slurm.%N.%j.out
#SBATCH -e /home-mscluster/bgriffiths/scan/slurm.%N.%j.err
srun ./scan/scan_mpi 16