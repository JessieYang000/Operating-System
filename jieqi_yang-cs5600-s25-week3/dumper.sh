#!/bin/bash
# Ensure the script runs using Bash

# Function to check the return code and display an appropriate message
check_error_code() {
    if [ $? -eq 0 ]; then
        echo "Success: Program executed without errors."
    else
        echo "Error: Program returned an error code."
    fi
}

# Call `arger` with no arguments (should trigger an error)
echo "Test 1: No arguments"
./arger
check_error_code
echo ""

# Call `arger` with invalid flag (should trigger an error)
echo "Test 2: Invalid flag"
./arger -x hello world
check_error_code
echo ""

# Call `arger` with valid `-u` flag
echo "Test 3: Uppercase conversion"
./arger -u hello world
check_error_code
echo ""

# Call `arger` with valid `-cap` flag
echo "Test 4: Title case conversion"
./arger -cap HELLO world
check_error_code
echo ""

# Call `arger` with valid `-l` flag
echo "Test 5: Lowercase conversion"
./arger -l HELLO WORLD
check_error_code
echo ""

