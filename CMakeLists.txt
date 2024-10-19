cmake_minimum_required(VERSION 3.30)

project(NcursesSnake VERSION 0.3.0.1 LANGUAGES CXX)

add_library(target_comp_flags INTERFACE)
target_compile_features(target_comp_flags INTERFACE cxx_std_20)

add_subdirectory(intlibs)

add_executable(NcursesSnake main.cpp)

target_link_libraries(NcursesSnake PUBLIC intlibs target_comp_flags -lncursesw)

target_include_directories(NcursesSnake PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )