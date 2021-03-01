cmake_minimum_required(VERSION 3.6.0)

set (TOP_PROJECT_NAME "GraphPipe")
project(${TOP_PROJECT_NAME})

set(OPT_CPPFLAGS " -fsanitize=address")

#set(C_FLAGS_WARN "-Werror=uninitialized -Werror=return-type -W")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS_WARN} ${OPT_CPPFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${C_FLAGS_WARN} ${OPT_CPPFLAGS}")

message (">>> ===============build start===================")
message (">>> ===============           ==================")
message (">>> ============================================")

message("    C++ Compiler:"           ${CMAKE_CXX_COMPILER})
message("    C++ flags (Release):"    ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE})
message("    C++ flags (Debug):"      ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
message("    C Compiler:"             ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1})
message("    C flags (Release):"      ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE})
message("    C flags (Debug):"        ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG})
message("    Linker flags (Release):" ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})
message("    Linker flags (Debug):"   ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
message("    definitions:"            ${COMPILE_DEFINITIONS})

message("    ranlib: "            ${CMAKE_RANLIB})
message("    ar    : "            ${CMAKE_AR}) 

set (TOP_PROJECT_PATH ${${TOP_PROJECT_NAME}_SOURCE_DIR})

# dump_value()

# include_directories(${TOP_PROJECT_PATH}
#     ./googletest/include)
# message("fengqian ${${${TOP_PROJECT_NAME}_lib}_SOURCE_DIR}/fw_base/include")

# set(SRC_LIST)

# list(APPEND SRC_LIST
#     ${TOP_PROJECT_PATH}/library//base/foundation/smessage.cpp
#     ${TOP_PROJECT_PATH}/library/base/foundation/string_atomizer.cpp
#     ${TOP_PROJECT_PATH}/library/base/common/common_log.cpp
#     ${TOP_PROJECT_PATH}/library/base/foundation/common_buffer.cpp
#     ${TOP_PROJECT_PATH}/library//base/foundation/common_struct.cpp
#     ${TOP_PROJECT_PATH}/library//base/foundation/common_message.cpp
#     )
    
# list(APPEND SRC_LIST    
#     main.cpp
#     left_right_value.cpp
#     threadpool.cpp
#     threadpool_test.cpp
#     packet_test.cpp
#     # build_adapt.cpp
#     # base/test_message.cpp
#     # base/test_queue.cpp
#     # base/test_atomizer.cpp
#     # base/test_smessage.cpp
#     # base/test_buffer.cpp
#     # base/test_struct.cpp
#     )

add_subdirectory(framework)
add_subdirectory(test)

# link_directories(./googletest/lib/osx)

# set(DNA_LINK_LIBRARIES
#     gmock
#     gmock_main
#     gtest
#     gtest_main
#     )

# add_executable(Qctest ${SRC_LIST})
# target_link_libraries (Qctest ${DNA_LINK_LIBRARIES})