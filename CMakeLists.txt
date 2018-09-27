cmake_minimum_required (VERSION 3.10)
project(OnlineJudge CXX)
include(Config.cmake)
include(CTest)

set_cxx_standard(14)
# set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_COMPILER "g++")

add_compile_options("-Wall" "-Wno-reorder" "-O0")
# add_compile_options("-ferror-limit=4")

file(GLOB SRCS "*.cpp")
foreach(SRC ${SRCS})
  STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" NAME ${SRC})
  add_executable(${NAME} ${SRC})
  add_custom_target(${NAME}.run ${NAME})
endforeach()

