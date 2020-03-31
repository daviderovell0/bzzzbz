cmake_minimum_required(VERSION 2.8.9)
project (spi_test)
add_executable(spi_test spi_test.cpp gpio-sysfs.cpp MCP3008Comm.cpp)

set (CMAKE_CXX_STANDARD 11)
find_package (Threads)
TARGET_LINK_LIBRARIES(spi_test rt ${CMAKE_THREAD_LIBS_INIT})
