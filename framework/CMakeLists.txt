cmake_minimum_required(VERSION 3.6.0)

project(${TOP_PROJECT_NAME}_framework)

set(SRC_LIST)

list(APPEND SRC_LIST 
    ./threadpool.cpp)

add_library(${TOP_PROJECT_NAME}_framework STATIC ${SRC_LIST})