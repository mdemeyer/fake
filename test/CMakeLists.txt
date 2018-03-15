find_package(GTest REQUIRED)

add_executable(tests
    src/main.cpp
    src/test_namegenerator.cpp
    ../src/namegenerator.cpp
    ../src/namegenerator.hpp
    )
target_link_libraries(tests
    GTest::GTest
    )

target_include_directories(tests
    PRIVATE
        ../src
)


gtest_discover_tests(tests TEST_PREFIX new:)
