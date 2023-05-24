cmake_minimum_required(VERSION 3.1)

project(
  ostep-cpu-api
  VERSION 1.0
  LANGUAGES C)

add_executable(system system.c)
add_executable(context context.c)
