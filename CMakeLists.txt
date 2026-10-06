cmake_minimum_required(VERSION 3.16)
project(Lab2_OS LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project requires Windows and WinAPI.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER "${CONFIG}" CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin")
endforeach()

add_library(array_helper SHARED array_helper.cpp array_helper.h)
target_compile_definitions(array_helper PRIVATE ARRAY_HELPER_EXPORTS)
set_target_properties(array_helper PROPERTIES PREFIX "")

add_executable(Lab2 main.cpp array_helper.h)
target_link_libraries(Lab2 PRIVATE kernel32)
add_dependencies(Lab2 array_helper)