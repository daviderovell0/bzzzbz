find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (../src
                     ${Boost_INCLUDE_DIRS}
                     )
add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (aptests testAudioProcessing.cpp)
target_link_libraries (aptests
                       AudioProcessing
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )