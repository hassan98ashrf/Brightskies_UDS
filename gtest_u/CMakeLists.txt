cmake_minimum_required(VERSION 3.25.1)

# Project name
project(TEST C CXX)

# Specify the C++ standard
set(CMAKE_C_STANDARD 99 )
set(CMAKE_CXX_STANDARD 11 ) 


# Add Google Test
add_subdirectory(googletest)

# Add the test executable
add_executable(test_example test/UDS_TestCases.cpp mock/APP_UDS_Mock.cpp  )

#target_include_directories(test_example PUBLIC inc/)
#target_include_directories(test_example PUBLIC mock/)

add_library(example src/APP_UDS_Diag.c)
# Add the C source file


# Link the test executable with Google Test and your C library
target_link_libraries(test_example gtest gtest_main example)



# Enable testing
enable_testing()

# Add the test
add_test(NAME example_test COMMAND test_example)
