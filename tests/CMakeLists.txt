include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(base_tests
    base_test.cc
)

target_link_libraries(base_tests
    PRIVATE
        cfxbase
        GTest::gmock_main
)
