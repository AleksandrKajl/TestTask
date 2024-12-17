cmake_minimum_required(VERSION 3.16)

project(TestTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/init_jobj.cpp
    src/init_jobj.h
    src/jobj.cpp
    src/jobj.h
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

