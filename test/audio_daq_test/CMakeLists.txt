# Build the audio test program
cmake_minimum_required(VERSION 3.10)

#set the project name
project(audioin)

# add the executable
add_executable(audioin audioin.c)
target_link_libraries(audioin jack)