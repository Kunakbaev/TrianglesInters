find_package(GTest REQUIRED)

add_executable(bruteforce_solution_tests tests.cpp)

target_compile_options(bruteforce_solution_tests PRIVATE -DNO_LOG -DTESTS_)
set_target_properties( bruteforce_solution_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

target_link_libraries(bruteforce_solution_tests PRIVATE
  GTest::gtest
  GTest::gtest_main
  spdlog::spdlog
  Threads::Threads
)
