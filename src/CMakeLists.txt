# Build main program

cmake_minimum_required(VERSION 3.10)
project (bz)

# Chain kiss-fft build since library needed for audio processing  
add_subdirectory (kiss-fft) 
add_library (AudioProcessing AudioProcessing.cpp AudioProcessing.h)
add_executable(bz main.cpp shader_utils.cpp AudioProcessing.cpp gpio-sysfs.cpp MCP3008Comm.cpp)

set (CMAKE_CXX_STANDARD 11)
find_package (Threads)
target_link_libraries(bz rt m ${CMAKE_THREAD_LIBS_INIT} 
                      jack GL glut GLEW kiss-fft
                      AudioProcessing)