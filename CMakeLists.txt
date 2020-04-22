cmake_minimum_required(VERSION 3.5)

project(Transport LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -pedantic")

add_executable(Transport main.cpp manager.cpp database.cpp stop.cpp bus.cpp request.cpp response.cpp parser.cpp)
