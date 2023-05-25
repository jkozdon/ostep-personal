cmake_minimum_required(VERSION 3.1)

project(
  ostep-cpu-api
  VERSION 1.0
  LANGUAGES C)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(wcat wcat.c)
