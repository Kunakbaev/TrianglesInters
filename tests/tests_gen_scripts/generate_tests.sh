#!/bin/bash

# Simple test generation script
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="$(dirname "$SCRIPT_DIR")"
OUTPUT_BASE="$TESTS_DIR/tests_data"

# Test sizes
SMALL=100
MEDIUM=10000
LARGE=200000

# # Create output directories
# mkdir -p "$OUTPUT_BASE/small_tests"
# mkdir -p "$OUTPUT_BASE/medium_tests" 
# mkdir -p "$OUTPUT_BASE/large_tests"

echo "Generating tests..."

# Generate small tests
echo "Small tests..."
cd "$TESTS_DIR/tests_gen_scripts/in_one_plane"
python3 gen_equilaterals.py $SMALL "small_tests/equilaterals"
python3 gen_many_inters.py $SMALL "small_tests/many_inters"
python3 gen_no_inters.py $SMALL "small_tests/no_inters"
python3 gen_random.py $SMALL "small_tests/random"

# Generate medium tests
echo "Medium tests..."
python3 gen_equilaterals.py $MEDIUM "medium_tests/equilaterals"
python3 gen_many_inters.py $MEDIUM "medium_tests/many_inters"
python3 gen_no_inters.py $MEDIUM "medium_tests/no_inters"
python3 gen_random.py $MEDIUM "medium_tests/random"

# Generate large tests
echo "Large tests..."
python3 gen_equilaterals.py $LARGE "large_tests/equilaterals"
python3 gen_many_inters.py  $LARGE "large_tests/many_inters"
python3 gen_no_inters.py    $LARGE "large_tests/no_inters"
python3 gen_random.py       $LARGE "large_tests/random"

echo "Done! Tests generated in: $OUTPUT_BASE"
