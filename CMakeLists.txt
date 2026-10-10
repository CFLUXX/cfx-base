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

set(BASE_LTO_COMPILE_FLAGS "")
set(BASE_LTO_LINK_FLAGS "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/d1trimfile:${CMAKE_SOURCE_DIR}/")
    set(BASE_LTO_COMPILE_FLAGS "/GL")
    set(BASE_LTO_LINK_FLAGS "/LTCG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=")
    set(BASE_LTO_COMPILE_FLAGS "-flto=thin")
    set(BASE_LTO_LINK_FLAGS "-flto=thin")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=")
    set(BASE_LTO_COMPILE_FLAGS "-flto=auto")
    set(BASE_LTO_LINK_FLAGS "-flto=auto")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASE_LTO_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BASE_LTO_LINK_FLAGS}")

find_package(fmt)

include(cmake/build.cmake)
add_subdirectory(tests)
