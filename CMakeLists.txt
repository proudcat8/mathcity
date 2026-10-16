cmake_minimum_required(VERSION 3.16)
project(mathcity CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(mathcity
  core/graph.cpp
  core/cost.cpp
  core/dijkstra.cpp
  core/services.cpp
  app/main.cpp
  app/serial.cpp
)

target_include_directories(mathcity PRIVATE core app)
target_link_libraries(mathcity PRIVATE raylib)
