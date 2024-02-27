project(CPULib)

add_executable(CPULibTest test.cpp)

target_link_libraries(CPULibTest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(CPULibTest)