CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(Game)

#使用默认的编译器
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)

LINK_DIRECTORIES(./Asura/bin)

ADD_SUBDIRECTORY(Asura)
ADD_SUBDIRECTORY(Sandbox)
ADD_SUBDIRECTORY(Asura-Editor)

find_library(ASURA libAsura.dll.a ${PROJECT_SOURCE_DIR}/cmake-build-release)
#find_library(GLEW glew32s ${PROJECT_SOURCE_DIR}/cmake-build-release)

find_package(yaml-cpp PATHS vendor/YAML_CPP)

find_package(box2d PATHS vendor/box2d)

MESSAGE(STATUS "yamll " ${YAML_CPP_LIBRARY_DIR})

message(STATUS "root This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "root This is SOURCE dir " ${PROJECT_SOURCE_DIR})
