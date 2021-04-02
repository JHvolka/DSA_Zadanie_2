cmake_minimum_required(VERSION 3.16)
project(Zadanie_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zadanie_2 main.cpp RB_TREE.h RB_TREE.cpp AvlTree.h avlbst.h bst.h HashRobin.cpp HashRobin.h)


if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Zadanie_2 PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(Zadanie_2 PUBLIC DEBUG=0)
endif()