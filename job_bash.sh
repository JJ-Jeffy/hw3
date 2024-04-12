#!/bin/bash
rm -r build
module load contrib upcxx/2023.9.0
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=CC ..
cmake --build .
salloc -N 2 -A mp309 -t 10:00 --qos=interactive -C cpu srun -N 1 -n 1 ./kmer_hash_19 /global/cfs/cdirs/mp309/cs267-spr2020/hw3-datasets/smaller/small.txt test
cat test_0.dat | sort > solution.txt
diff solution.txt /global/cfs/cdirs/mp309/cs267-spr2020/hw3-datasets/smaller/small_solution.txt 