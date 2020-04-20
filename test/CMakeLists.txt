find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (../src
                     ${Boost_INCLUDE_DIRS}
                     )
add_definitions (-DBOOST_TEST_DYN_LINK)
list(APPEND BOOST_LIBS ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# executables
add_executable (APtest.o testAudioProcessing.cpp)
add_executable (MCPtest.o testMCP3008Comm.cpp)

# linking
target_link_libraries (APtest.o AudioProcessing ${BOOST_LIBS})
target_link_libraries (MCPtest.o MCP3008Comm ${BOOST_LIBS})