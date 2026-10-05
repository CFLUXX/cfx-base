cmake_minimum_required(VERSION 3.28.0)
project(cfxbase
    LANGUAGES CXX C
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options("/d1trimfile:${CMAKE_SOURCE_DIR}/")
else()
    add_compile_options("-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=")
endif()


find_package(fmt)

include(cmake/build.cmake)
add_subdirectory(tests)
