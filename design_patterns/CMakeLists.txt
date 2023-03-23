cmake_minimum_required(VERSION 3.24)
project(design_patterns)

set(CMAKE_CXX_STANDARD 23)

add_executable(abstract_factory abstract_factory.cpp)
add_executable(adapter adapter.cpp)
add_executable(builder builder.cpp)
add_executable(chain_of_responsibility chain_of_responsibility.cpp)
add_executable(factory_method factory_method.cpp)
add_executable(prototype prototype.cpp)
add_executable(singleton singleton.cpp)
add_executable(visitor visitor.cpp)
