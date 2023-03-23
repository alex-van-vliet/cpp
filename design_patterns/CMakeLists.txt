cmake_minimum_required(VERSION 3.24)
project(design_patterns)

set(CMAKE_CXX_STANDARD 23)

add_executable(abstract_factory abstract_factory.cpp)
add_executable(builder builder.cpp)
add_executable(factory_method factory_method.cpp)
