cmake_minimum_required(VERSION 3.10)
project (examples)
add_executable(ex_audio ../shader_utils.cpp ex_audio.cpp ../AudioProcessing.cpp)
add_executable(ex_audio_slow ../shader_utils.cpp ex_audio_slow.cpp ../AudioProcessing.cpp)
add_executable(ex_opengl ../shader_utils.cpp ex_opengl.cpp)
add_executable(ex_SPI ../shader_utils.cpp ex_SPI.cpp ../MCP3008Comm.cpp ../gpio-sysfs.cpp)

set (CMAKE_CXX_STANDARD 11)
find_package (Threads)
target_link_libraries(ex_audio rt m ${CMAKE_THREAD_LIBS_INIT} 
                      jack GL glut GLEW kiss-fft)
target_link_libraries(ex_audio_slow rt m ${CMAKE_THREAD_LIBS_INIT} 
                      jack GL glut GLEW kiss-fft)
target_link_libraries(ex_opengl rt m ${CMAKE_THREAD_LIBS_INIT} 
                      jack GL glut GLEW kiss-fft)
target_link_libraries(ex_SPI rt m ${CMAKE_THREAD_LIBS_INIT} 
                      jack GL glut GLEW kiss-fft)