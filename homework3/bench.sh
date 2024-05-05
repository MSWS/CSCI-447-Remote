#!/bin/bash

# Settings 
program="./Simulation"  # Path to your Simulation program
datafile="sampleFile.txt"  # Your input data file
outputfile="results.txt"  # Where output will be stored

# Ranges to test for A, B, and PreExempt
values_A=(2 3 5 7 10 20 30 40 50 60 70 80 90 100 250 500 1000)
values_B=(2 3 5 7 10 20 30 40 50 60 70 80 90 100 250 500 1000)
values_PreExempt=(0 1)

# Clear existing output file
echo "" > $outputfile

# Loop through all combinations
echo "A,B,PreExempt,End Time,Average Wait,Max Wait,Min Wait" >> $outputfile
for A in "${values_A[@]}"; do
    for B in "${values_B[@]}"; do
        for PreExempt in "${values_PreExempt[@]}"; do
            # Run the simulation
            results=$($program $datafile $A $B $PreExempt)

            # Extract relevant values using regular expressions
            # end_time=$(echo "$results" | grep -oP 'Start/end time: \K\d+(?=,)')
            end_time=$(echo "$results" | grep -oP 'Start/end time: \K\d+, \d+' | cut -d ',' -f2 | tr -d ' ')
            avg_wait=$(echo "$results" | grep -oP 'Ready time average: \K\d+\.\d+')
            max_wait=$(echo "$results" | grep -oP 'Max ready time: \K\d+')
            min_wait=$(echo "$results" | grep -oP 'Min ready time: \K\d+')

            echo "$A,$B,$PreExempt,$end_time,$avg_wait,$max_wait,$min_wait" >> $outputfile
            # Write results to output file
        done
    done
done
