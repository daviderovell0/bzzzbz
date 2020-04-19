# Build main code + unit tests for classes

cmake_minimum_required (VERSION 2.8)
project (TEST)
add_subdirectory (src) 
add_subdirectory (test)
enable_testing ()
add_test (NAME AudioProcessingTest COMMAND APtest.o)