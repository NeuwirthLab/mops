#!/bin/bash

# Load the Darshan module
spack load darshan-runtime@3.4.4
export DARSHAN_LOG_DIR_PATH=.

# Define the parameters
dims="10 100 1000 10000"
input_path="/hdd-share/generated-matrix/"
output_path="/hdd-share/result-matrix/"


export DARSHAN_ENABLE_NONMPI=1
# Loop through each combination of parameters
for d in $dims; do
  echo "Running program with --rows=$d --cols=$d"
  LD_PRELOAD=/home/luser/Repos/spack/opt/spack/linux-almalinux9-ivybridge/gcc-11.4.1/darshan-runtime-3.4.4-ykayvipyjq7hnrzq7qeqs5ekpn6nga4t/lib/libdarshan.so ../build/mops -i $input_path -o $output_path -c $d -r $d
  echo "Done"
done

