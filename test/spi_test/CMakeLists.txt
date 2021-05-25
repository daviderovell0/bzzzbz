cmake_minimum_required(VERSION 2.8.9)
project (spi_test)
add_executable(spi_test spi_test.cpp  ../../src/gpio-sysfs.cpp  ../../src/MCP3004Comm.cpp)

set (CMAKE_CXX_STANDARD 11)
find_package (Threads)
target_include_directories(spi_test PUBLIC ../../src)
TARGET_LINK_LIBRARIES(spi_test rt ${CMAKE_THREAD_LIBS_INIT})

