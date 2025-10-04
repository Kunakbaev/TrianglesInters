
find_package(Threads REQUIRED)

enable_testing()

add_subdirectory(utils)
add_subdirectory(point)
add_subdirectory(segment)
add_subdirectory(plane)
add_subdirectory(triangle)
add_subdirectory(bruteforce_solution)

add_test(NAME utils_tests         COMMAND utils_tests)
add_test(NAME point_tests         COMMAND point_tests)
add_test(NAME segment_tests       COMMAND segment_tests)
add_test(NAME plane_tests         COMMAND plane_tests)
add_test(NAME triangle_tests      COMMAND triangle_tests)
add_test(NAME bruteforce_solution COMMAND bruteforce_solution)

add_custom_target(run_all_tests
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  COMMENT "Running tests with CTest"
)
