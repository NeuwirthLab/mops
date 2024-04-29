## MOPS
### Build

Build flags:
* WITH_OPENMP Compile with OpenMP support for parallel execution
* WITH_TACO Use TACOs write function to store the generated matrix
* WITH_LIKWID Compile with LIKIWID support to profile performance of the compute kernel

The following demonstrates how to compile MOPS with OpenMP and LIKWID support.
```bash
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DWITH_OPENMP=yes -DWITH_LIKWID=yes .."
$ make
```

## Matrix Generation
```bash
$ ./mops --generator --g_read [PATH] --g_write [PATH] --cols [cols] --rows [rows] [--copy] [--tacoed]
```

## LIKWID Profiling
In order to generate performance counter backed data, run the MOPS computation with thread pinning and ```likwid-perfctr```
```bash
$ likwid-perfctr -C N -g MEM_DP -m  ./mops -i ./dense_matrix_1000x1000.mtx -o ./1000x1000.mtx -t 32
```
