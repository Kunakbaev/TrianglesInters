cmake_minimum_required(VERSION 1.14)
project(TrianglesIntersections)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_LINK_FLAGS "-lm")

# --------------------------------------------

add_library(my_project_includes INTERFACE)

target_include_directories(my_project_includes INTERFACE
  ${CMAKE_SOURCE_DIR}/3dPartyModules/LoggerLib/include
  ${CMAKE_SOURCE_DIR}/realizations/geom_primitives
  ${CMAKE_SOURCE_DIR}/realizations/bruteforce_solution
  ${CMAKE_SOURCE_DIR}/realizations
)

# ---------------------------------------------

set(COMMON_CXX_FLAGS "-lm -ggdb3 -std=c++17 -Werror -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # removes assert checks and log lib output
  add_compile_options(-O3 -DNDEBUG -DNO_LOG)
  message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(${COMMON_CXX_FLAGS})
  add_compile_options(-g -O0 -D_DEBUG)
  message(STATUS "Building in Debug mode")
endif()


# --------------------------------------------

# 1. Сначала генерируем config файл
include(CMakePackageConfigHelpers)
configure_package_config_file(
  3dPartyModules/LoggerLib/cmake/my_loglibConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/my_loglibTargets.cmake
  INSTALL_DESTINATION lib/cmake/my_loglib
)

# 2. Указываем КОНКРЕТНЫЙ путь к файлу
set(my_loglib_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# 3. Добавляем subdirectory
add_subdirectory(3dPartyModules/LoggerLib)


# --------------------------------------------

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt CONTENT "
Available targets:
  * naive optimized_BVH_solution - two solutions of main task. How they work: first number of triangles is expected, than set of 3d triangles in stated quantity. Each triangle is described by 6 numbers (not necessary integers). As an output it produces list of triangles indices that intersect with at least one other triangle. \"naive\" - is a slow solution, works in O(n^2) (where \"n\" is number of triangles) by iterating through every pair of triangles. optimized_BVH_solution uses BVH_tree (BVH stands for bounding volume hierarchy), it's faster in some cases.
  * These are targets which test methods of one particular class (e.g. target segment_unit_test tests correctness of segment_t class)
usage examples:
    * utils_unit_test
    * point_tests
    * segment_unit_test
    * plane_unit_test
    * triangle_unit_test
    * bruteforce_solution_unit_test
    * AABB_unit_test 
    * optimized_BVH_solution_unit_test
  * to set build type:       cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug
  * example of building and running usecase targets:
    1) naive solution
    to build naive target: cmake --build bin --target naive
    and to run it: ./bin/usecase/naive
    2) optimized with BVH tree solution
    to build: cmake --build bin --target optimized_BVH_solution
    to run it: ./bin/usecase/optimized_BVH_solution
  * to run triangles_tests: ./bin/unit_tests/triangle/triangle_unit_test
  * to run all tests (uses CTest): make -C ./bin/unit_tests/ run_all_tests
")

add_custom_target(show_help_msg
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt
)

add_subdirectory(usecase)
add_subdirectory(unit_tests)
