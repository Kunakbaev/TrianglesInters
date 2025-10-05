
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

# Define a function to create test targets
function(create_unit_test test_name source_file)
  add_executable(${test_name} ${source_file})

  target_compile_options(${test_name} PRIVATE -DNO_LOG -DTESTS_)
  set_target_properties( ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

  target_link_libraries(${test_name} PRIVATE
    GTest::gtest
    GTest::gtest_main
    my_loglib
    Threads::Threads
  )
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

create_unit_test(utils               utils_tests.cpp)
create_unit_test(point               point_tests.cpp)
create_unit_test(segment             segment_tests.cpp)
create_unit_test(plane               plane_tests.cpp)
create_unit_test(triangle            triangle_tests.cpp)
create_unit_test(bruteforce_solution bruteforce_solution_tests.cpp)

add_custom_target(run_all_tests
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  COMMENT "Running tests with CTest"
)
