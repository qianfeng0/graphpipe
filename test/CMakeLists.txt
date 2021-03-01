cmake_minimum_required(VERSION 3.6.0)

project(${TOP_PROJECT_NAME}_test)

set(SRC_LIST)
list(APPEND SRC_LIST 
    left_right_value.cpp
    main.cpp
    packet_test.cpp
    threadpool_test.cpp)

include_directories(${TOP_PROJECT_PATH}/googletest/include
    ${TOP_PROJECT_PATH}/framework)

link_directories(${TOP_PROJECT_PATH}/googletest/lib/osx)
set(DNA_LINK_LIBRARIES)
list(APPEND DNA_LINK_LIBRARIES
    gmock
    gmock_main
    gtest
    gtest_main)

list(APPEND DNA_LINK_LIBRARIES
    ${TOP_PROJECT_NAME}_framework
)

add_executable(Qctest ${SRC_LIST})
target_link_libraries (Qctest ${DNA_LINK_LIBRARIES})