cmake_minimum_required(VERSION 1.14)
project(TrianglesIntersections)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_LINK_FLAGS "-lm")

include_directories(common_includes)
include_directories(realizations/geom_primitives)
include_directories(realizations/bruteforce_solution)
include_directories(realizations)

# --------------------------------------------

# Add subdirectory for spdlog
add_subdirectory(3dPartyModules/spdlog)

# --------------------------------------------

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt CONTENT "
Available targets:
  * usecase - runs main target, it's an actual task solver: first expects number of triangles, than set of 3d triangles in stated quantity. Each triangle is described by 6 numbers (not necessary integers). As an output it produces list of triangles indices that intersect with at least one other triangle.
  * utils_tests, point_tests, segment_tests, plane_tests, triangle_tests, bruteforce_solution_tests - these targets test methods of one particular class (e.g. segment_tests tests correctness of segment_t class)
usage examples:
  * to set build type:       cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug
  * to build usecase target: cmake --build bin --target usecase
  * and to run usecase: ./bin/usecase/usecase
  * to run triangles_tests: ./bin/tests/triangle/triangle_tests
  * to run all tests (uses CTest): make -C ./bin/tests/ run_all_tests
")

add_custom_target(show_help_msg
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt
)

add_subdirectory(usecase)
add_subdirectory(tests)
