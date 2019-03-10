cmake_minimum_required(VERSION 3.13)
project(SafePtr)

set(CMAKE_CXX_STANDARD 17)

add_executable(SafePtr main.cpp SafePtr.cpp SafePtr.h)
#add_library(SafePtr main.cpp SafePtr.cpp SafePtr.h)

target_include_directories(SafePtr PUBLIC ./inc)
target_link_directories(SafePtr PUBLIC ./lib)
target_link_libraries(SafePtr libUtils.a)
target_link_libraries(SafePtr libLogger.a)