cmake_minimum_required(VERSION 3.8)
project(fake VERSION 0.0.1 LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(Boost 1.55 REQUIRED
    COMPONENTS
         program_options
         system
    )

add_executable(fake
    src/main.cpp
    )

target_compile_features(fake PRIVATE cxx_std_17)
target_compile_options(fake PRIVATE -Werror -Wall -Wextra)

target_link_libraries(fake
    PRIVATE
        Boost::program_options
        Boost::system
        Threads::Threads
    )
