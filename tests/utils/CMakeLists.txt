find_package(GTest REQUIRED)

add_executable(utils_tests tests.cpp)

target_compile_options(utils_tests PRIVATE -DNO_LOG -DTESTS_)
set_target_properties( utils_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

target_link_libraries(utils_tests PRIVATE
  GTest::gtest
  GTest::gtest_main
  spdlog::spdlog
  Threads::Threads
)
