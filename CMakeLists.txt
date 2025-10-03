cmake_minimum_required(VERSION 1.14)
project(TrianglesIntersections)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_LINK_FLAGS "-lm")

include_directories(common_includes)
include_directories(realizations/geom_primitives)
include_directories(realizations)

# --------------------------------------------

# Add subdirectory for spdlog
add_subdirectory(3dPartyModules/spdlog)

# --------------------------------------------

add_subdirectory(usecase)
add_subdirectory(tests)
