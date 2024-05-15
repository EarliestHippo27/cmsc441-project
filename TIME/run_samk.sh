#!/bin/bash

# Number of times to run the C++ application
num_runs=12 # 12
num_tests=15 # 15
num_cut=12

# Define command-line arguments
size=2
# fn="data.csv"

# Run the application multiple times with different arguments
for ((k = 1; k < num_cut; k++)); do
	for ((j = 0; j < num_tests; j++)); do
		for ((i = 0; i <= num_runs; i++)); do
			cutoff=$((2**k))
			filename="data${cutoff}.csv"
			size=$((2**i))
			./samk "$size" "$filename" "$cutoff" &
			wait
		done
	done
done

