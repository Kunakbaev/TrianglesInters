#!/usr/bin/env python3
import os
import subprocess
import time
import statistics
import math
from collections import defaultdict
import sys

def run_test_with_timing(executable, test_file):
    """Run executable with test file content as stdin and return time"""
    try:
        with open(test_file, 'r') as f:
            test_content = f.read()
        
        start_time = time.time()
        result = subprocess.run([executable], 
                              input=test_content,
                              capture_output=True, 
                              text=True, 
                              timeout=10)  # 10 second timeout
        end_time = time.time()
        
        execution_time = (end_time - start_time) * 1000  # Convert to milliseconds
        return execution_time, result.stdout.strip()
    except subprocess.TimeoutExpired:
        return 5000, "TIMEOUT"  # Mark as 5+ seconds
    except Exception as e:
        return 0, f"ERROR: {str(e)}"

def print_statistics(times, test_type):
    """Print statistics for a given test type"""
    if not times:
        return
    
    mean_time = statistics.mean(times)
    median_time = statistics.median(times)
    variance = statistics.variance(times) if len(times) > 1 else 0
    std_dev = math.sqrt(variance)
    min_time = min(times)
    max_time = max(times)
    
    print(f"  {test_type}:")
    print(f"    Count: {len(times)}")
    print(f"    Mean: {mean_time:.2f} ms")
    print(f"    Median: {median_time:.2f} ms")
    print(f"    Std Dev: {std_dev:.2f} ms")
    print(f"    Min: {min_time:.2f} ms")
    print(f"    Max: {max_time:.2f} ms")
    print(f"    Range: {max_time - min_time:.2f} ms")

def main():
    test_data_dir = "tests_data/in_one_plane"
    optimized_solution = "../bin/usecase/optimized_BVH_solution"
    
    if not os.path.exists(optimized_solution):
        print(f"Error: {optimized_solution} not found!")
        sys.exit(1)
    
    # Collect all large test files grouped by test type
    test_types = defaultdict(list)
    size_path = os.path.join(test_data_dir, "large_tests")
    
    if not os.path.exists(size_path):
        print(f"Error: {size_path} not found!")
        sys.exit(1)
    
    for test_type in os.listdir(size_path):
        test_type_path = os.path.join(size_path, test_type)
        if not os.path.isdir(test_type_path):
            continue
        
        for test_file in sorted(os.listdir(test_type_path)):
            if test_file.endswith('.dat'):
                test_file_path = os.path.join(test_type_path, test_file)
                test_types[test_type].append((test_file, test_file_path))
    
    print(f"Found large test files by type:")
    for test_type, files in test_types.items():
        print(f"  {test_type}: {len(files)} files")
    print("\nBenchmarking optimized solution...\n")
    
    # Store timing results per test type
    timing_results = defaultdict(list)
    all_times = []
    slow_tests = []
    max_time = 0
    max_test = None
    
    # Run tests for each type
    for test_type, test_files in test_types.items():
        print(f"🔹 {test_type.upper()} tests:")
        for test_file, test_file_path in test_files:
            print(f"  Running: {test_file}", end=" - ")
            
            execution_time, output = run_test_with_timing(optimized_solution, test_file_path)
            timing_results[test_type].append(execution_time)
            all_times.append(execution_time)
            
            if execution_time > max_time:
                max_time = execution_time
                max_test = f"{test_type}/{test_file}"
            
            if execution_time >= 5000:  # 5 seconds in milliseconds
                slow_tests.append(f"{test_type}/{test_file} ({execution_time:.2f}ms)")
                print(f"⏰ TOO SLOW: {execution_time:.2f}ms")
            else:
                print(f"{execution_time:.2f}ms")
        print()
    
    # Print detailed statistics per test type
    print("="*70)
    print("DETAILED STATISTICS PER TEST TYPE")
    print("="*70)
    
    for test_type in sorted(timing_results.keys()):
        print_statistics(timing_results[test_type], test_type)
        print()
    
    # Print overall statistics
    print("="*70)
    print("OVERALL STATISTICS FOR ALL LARGE TESTS")
    print("="*70)
    
    if all_times:
        mean_time = statistics.mean(all_times)
        median_time = statistics.median(all_times)
        variance = statistics.variance(all_times) if len(all_times) > 1 else 0
        std_dev = math.sqrt(variance)
        
        print(f"Total tests: {len(all_times)}")
        print(f"Mean time: {mean_time:.2f} ms")
        print(f"Median time: {median_time:.2f} ms")
        print(f"Variance: {variance:.2f} ms²")
        print(f"Standard deviation: {std_dev:.2f} ms")
        print(f"Min time: {min(all_times):.2f} ms")
        print(f"Max time: {max_time:.2f} ms")
        
        if max_test:
            print(f"\n📊 Slowest test overall: {max_test} ({max_time:.2f} ms)")
        
        # Print slowest per type
        print(f"\n📊 Slowest test per type:")
        for test_type in sorted(timing_results.keys()):
            if timing_results[test_type]:
                slowest = max(timing_results[test_type])
                print(f"  {test_type}: {slowest:.2f} ms")
        
        if slow_tests:
            print(f"\n⚠️  Tests slower than 5 seconds ({len(slow_tests)}):")
            for test in slow_tests:
                print(f"  {test}")
        else:
            print(f"\n✅ All tests completed under 5 seconds")
        
        # Print time distribution overall
        print(f"\n📈 Overall time distribution:")
        fast = len([t for t in all_times if t < 100])
        medium = len([t for t in all_times if 100 <= t < 1000])
        slow = len([t for t in all_times if t >= 1000])
        print(f"  < 100ms: {fast} tests")
        print(f"  100-1000ms: {medium} tests")
        print(f"  >= 1000ms: {slow} tests")
        
        # Print time distribution per type
        print(f"\n📊 Time distribution per type:")
        for test_type in sorted(timing_results.keys()):
            times = timing_results[test_type]
            fast = len([t for t in times if t < 100])
            medium = len([t for t in times if 100 <= t < 1000])
            slow = len([t for t in times if t >= 1000])
            print(f"  {test_type}: <100ms:{fast} | 100-1000ms:{medium} | >=1000ms:{slow}")
    
    else:
        print("No test results to analyze!")

if __name__ == "__main__":
    main()

"""

cur best:

equilaterals: 991.81 ms
many_inters: 846.47 ms
no_inters: 864.09 ms
random: 4275.03 ms

"""
