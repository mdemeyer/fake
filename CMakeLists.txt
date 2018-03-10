cmake_minimum_required(VERSION 3.8)
project(fake VERSION 0.0.1 LANGUAGES CXX)

add_executable(fake src/main)
target_compile_features(fake PRIVATE cxx_std_17)
