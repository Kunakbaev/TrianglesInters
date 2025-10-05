find_package(GTest REQUIRED)

add_executable(point_tests tests.cpp)

target_compile_options(point_tests PRIVATE -DNO_LOG -DTESTS_)
set_target_properties( point_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

target_link_libraries(point_tests PRIVATE
  GTest::gtest
  GTest::gtest_main
  my_loglib
  Threads::Threads
)
