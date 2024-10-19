add_library(intlibs gutils.cpp gobjs.cpp ghandle.cpp gflow.cpp ginit.cpp)

target_include_directories(intlibs INTERFACE
                          ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(intlibs PUBLIC target_comp_flags)