add_library(cfxbase STATIC)

file(GLOB CFX_RESULT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/result/impl/*.cc
    ${CMAKE_SOURCE_DIR}/src/coroutine/impl/*.cc
)

file(GLOB CFX_RESULT_MODULES
    ${CMAKE_SOURCE_DIR}/src/result/*.cppm
    ${CMAKE_SOURCE_DIR}/src/result/partition/*.cppm
    ${CMAKE_SOURCE_DIR}/src/coroutine/*.cppm
    ${CMAKE_SOURCE_DIR}/src/coroutine/partition/*.cppm
)

message("${CFX_RESULT_SOURCES}")

target_link_libraries(cfxbase
    PUBLIC
        fmt::fmt
)


target_sources(cfxbase
    PRIVATE
        ${CFX_RESULT_SOURCES}

    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_SOURCE_DIR}
        FILES ${CFX_RESULT_MODULES}

)


