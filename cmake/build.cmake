add_library(cfxresult STATIC)

file(GLOB CFX_RESULT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/result/impl/*.cc
)

file(GLOB CFX_RESULT_MODULES
    ${CMAKE_SOURCE_DIR}/src/result/*.cppm
    ${CMAKE_SOURCE_DIR}/src/result/partition/*.cppm
)

target_sources(cfxresult
    PRIVATE
        ${CFX_RESULT_SOURCES}
)

target_sources(cfxresult
    PRIVATE
        FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_SOURCE_DIR}
        FILES ${CFX_RESULT_MODULES}
)
