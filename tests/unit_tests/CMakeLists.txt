enable_testing()

add_executable(unit_tests
    test_sorting.cpp
)

target_link_libraries(unit_tests
    gtest
    gtest_main
)
