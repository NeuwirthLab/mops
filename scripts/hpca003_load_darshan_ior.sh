#!/bin/bash

# Define the parameters
tsize="4k 32k 128k 512k 1m 2m"
bsize="200k 20m 1g 2g"

output_path="/srv/hdd/ior_out/test.out"

. /home/luser/Repos/spack/share/spack/setup-env.sh

spack load darshan-runtime@3.4.4
export DARSHAN_LOG_DIR_PATH=.
export DARSHAN_ENABLE_NONMPI=1

spack load ior@3.3.0


# Loop through each combination of parameters
for t in $tsize; do
    for b in $bsize; do
            echo "Running program with -t = $t -b = $b"
            LD_PRELOAD=/home/luser/Repos/spack/opt/spack/linux-almalinux9-ivybridge/gcc-11.4.1/darshan-runtime-3.4.4-3elp6ce775ewdazycqrxmkbyi6em2x76/lib/libdarshan.so ior -a POSIX -b "$b" -t "$t" -e -C -o "$output_path" --posix.odirect
    done
done