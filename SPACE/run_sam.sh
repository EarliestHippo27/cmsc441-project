#!/bin/bash

# Number of times to run the C++ application
num_runs=12
num_tests=15

# Define command-line arguments
size=2
fn="sam_size_data.csv"

# Run the application multiple times with different arguments
for ((j = 0; j < num_tests; j++)); do
	for ((i = 0; i <= num_runs; i++)); do
		size=$((2**i))
		./sam_size "$size" "$fn" &
		wait
	done
done

