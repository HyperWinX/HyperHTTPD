cmake_minimum_required(VERSION 3.28)

project(hyperhttpd VERSION 0.0 LANGUAGES CXX)

find_package(Boost REQUIRED)

add_library(libhyperhttpd STATIC
  src/server.cpp)
target_include_directories(libhyperhttpd PUBLIC
  include)
set_target_properties(libhyperhttpd PROPERTIES COMPILE_FLAGS "-O3 -flto=thin")

add_executable(server main.cpp)
target_link_libraries(server libhyperhttpd pthread boost_thread)
set_target_properties(server PROPERTIES COMPILE_FLAGS "-O3 -flto=thin")