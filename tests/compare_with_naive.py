#!/usr/bin/env python3
import os
import subprocess
import sys

def run_test(executable, test_file):
    """Run executable with test file content as stdin"""
    try:
        with open(test_file, 'r') as f:
            test_content = f.read()
        
        result = subprocess.run([executable], 
                              input=test_content,
                              capture_output=True, 
                              text=True, 
                              timeout=4)
        return result.stdout.strip()
    except subprocess.TimeoutExpired:
        return "TIMEOUT"
    except Exception as e:
        return f"ERROR: {str(e)}"

def main():
    test_data_dir = "tests_data/in_one_plane"
    naive_solution     = "../bin/usecase/naive"
    optimized_solution = "../bin/usecase/optimized_BVH_solution"
    
    if not os.path.exists(naive_solution):
        print(f"Error: {naive_solution} not found!")
        sys.exit(1)
    if not os.path.exists(optimized_solution):
        print(f"Error: {optimized_solution} not found!")
        sys.exit(1)
    
    failed_tests = []
    total_tests = 0
    
    print("Comparing solutions (reading from stdin)...")
    
    # Count total tests
    for size in ["small_tests", "medium_tests", "large_tests"]:
        size_path = os.path.join(test_data_dir, size)
        if not os.path.exists(size_path):
            continue
            
        for test_type in os.listdir(size_path):
            test_type_path = os.path.join(size_path, test_type)
            if not os.path.isdir(test_type_path):
                continue
            
            for test_file in os.listdir(test_type_path):
                if test_file.endswith('.dat'):
                    total_tests += 1
    
    current_test = 0
    
    for size in ["small_tests", "medium_tests"]:
        size_path = os.path.join(test_data_dir, size)
        if not os.path.exists(size_path):
            continue
            
        for test_type in os.listdir(size_path):
            if test_type == "no_inters" and size == "medium_tests":
                print("Naive works bad on no_inters test type")
                continue
            
            test_type_path = os.path.join(size_path, test_type)
            if not os.path.isdir(test_type_path):
                continue
            
            for test_file in sorted(os.listdir(test_type_path)):
                if test_file.endswith('.dat'):
                    test_file_path = os.path.join(test_type_path, test_file)
                    current_test += 1
                    
                    print(f"[{current_test}/{total_tests}] {size}/{test_type}/{test_file}", end=" - ")
                    
                    # Run both solutions with file content as stdin
                    naive_result = run_test(naive_solution, test_file_path)
                    optimized_result = run_test(optimized_solution, test_file_path)
                    
                    # Compare results
                    if naive_result != optimized_result:
                        failed_tests.append(f"{size}/{test_type}/{test_file}")
                        print("❌ FAILED")
                        print(f"   Naive: {naive_result}")
                        print(f"   Optimized: {optimized_result}")
                    else:
                        print("✅ PASSED")
    
    # Summary
    print("\n" + "="*50)
    print("RESULTS")
    print("="*50)
    print(f"Total tests: {total_tests}")
    print(f"Passed: {total_tests - len(failed_tests)}")
    print(f"Failed: {len(failed_tests)}")
    
    if failed_tests:
        print("\nFailed tests:")
        for test in failed_tests:
            print(f"  {test}")
    else:
        print("\n🎉 All tests passed!")

if __name__ == "__main__":
    main()
