add_library(cfxbase STATIC)

file(GLOB CFX_BASE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/result/impl/*.cc
    ${CMAKE_SOURCE_DIR}/src/coroutine/impl/*.cc
    ${CMAKE_SOURCE_DIR}/src/filesystem/impl/*.cc
)

file(GLOB CFX_BASE_MODULES
    ${CMAKE_SOURCE_DIR}/src/result/partition/*.cppm
    ${CMAKE_SOURCE_DIR}/src/result/*.cppm
    ${CMAKE_SOURCE_DIR}/src/coroutine/partition/*.cppm
    ${CMAKE_SOURCE_DIR}/src/coroutine/*.cppm
    ${CMAKE_SOURCE_DIR}/src/filesystem/*.cppm
    ${CMAKE_SOURCE_DIR}/src/filesystem/partition/*.cppm
)

message("src__: ${CFX_BASE_SOURCES}")
message("modules__: ${CFX_BASE_MODULES}")

target_link_libraries(cfxbase
    PUBLIC
        fmt::fmt
)

target_include_directories(cfxbase
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_sources(cfxbase
    PRIVATE
        ${CFX_BASE_SOURCES}

    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_SOURCE_DIR}
        FILES ${CFX_BASE_MODULES}

)


