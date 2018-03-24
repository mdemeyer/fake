cmake_minimum_required(VERSION 3.10)
project(fake VERSION 0.0.1 LANGUAGES CXX)

enable_testing()

find_package(Threads REQUIRED)
find_package(Boost 1.55 REQUIRED
    COMPONENTS
         program_options
         system
    )

add_executable(fake
    src/namegenerator.cpp
    src/namegenerator.hpp
    src/parseoptions.cpp
    src/parseoptions.hpp
    src/progresscounter.hpp
    src/main.cpp
    )

target_compile_features(fake PRIVATE cxx_std_17)
target_compile_options(fake PRIVATE -Werror -Wall -Wextra)

target_include_directories(fake
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(fake
    PRIVATE
        Boost::program_options
        Boost::system
        Threads::Threads
    )

add_subdirectory(test)
