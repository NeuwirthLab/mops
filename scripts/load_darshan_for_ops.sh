#!/bin/bash

# Load the Darshan module
spack load darshan-runtime@3.4.4
export DARSHAN_LOG_DIR_PATH=.

# Define the parameters
dims="10 100 1000 10000"

export DARSHAN_ENABLE_NONMPI=1
# Loop through each combination of parameters
for d in $dims; do
  input_file="/srv/hdd/generated-matrix/dense_matrix_${d}x${d}.mtx"
  output_file="/srv/hdd/result-matrix/result_dense_matrix_${d}x${d}.mtx"
  echo "Running taco_ops with -i= $input_file -o=$output_file"
  LD_PRELOAD=/home/luser/Repos/spack/opt/spack/linux-almalinux9-ivybridge/gcc-11.4.1/darshan-runtime-3.4.4-3elp6ce775ewdazycqrxmkbyi6em2x76/lib/libdarshan.so /home/luser/Repos/mops/omp_build/mops -i "$input_file" -o "$output_file"
  echo "Done"
done

