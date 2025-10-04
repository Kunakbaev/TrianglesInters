find_package(GTest REQUIRED)

add_executable(segment_tests tests.cpp)

target_compile_options(segment_tests PRIVATE -DNO_LOG -DTESTS_)
set_target_properties( segment_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

target_link_libraries(segment_tests PRIVATE
  GTest::gtest
  GTest::gtest_main
  my_loglib
  Threads::Threads
)
