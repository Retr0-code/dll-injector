cmake_minimum_required(VERSION 3.17.0)

project(DLLInjector VERSION 0.1)

set(SOURCES
    src/injector.cpp
    src/main.cpp
)

set(HEADERS
    include/injector.hpp
)

option(UNICODE "Use unicode or wide char WinApi functions" ON)
if (UNICODE)
    add_compile_definitions(USE_WFUNCTIONS)
    add_link_options(-municode)
    add_compile_options(-municode)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
add_library(test_dll SHARED src/test_dll.cpp)
set_target_properties(test_dll PROPERTIES SUFFIX ".dll")
