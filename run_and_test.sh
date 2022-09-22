#!/bin/bash

# A quick helper script to compile that compiles, tests, and runs valgrind for the check_whitespace.c
# files (To avoid needing to run three commands to check after every change)

# Compile the check_whitespace.c
gcc -g -Wall -o check_whitespace main.c check_whitespace.c

# Run the gtest suite for the check_whitespace.c
g++ -Wall -g -o check_whitespace_test check_whitespace.c check_whitespace_test.cpp -lgtest

# Run the valgrind checks on the check_whitespace.c file
valgrind --leak-check=full ./check_whitespace
