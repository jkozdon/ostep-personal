cmake_minimum_required(VERSION 3.1)

project(
  ostep-cpu-api
  VERSION 1.0
  LANGUAGES C)

add_executable(var var.c)
add_executable(file file.c)
add_executable(exec exec.c)
add_executable(wait wait.c)
add_executable(close close.c)
add_executable(pipe pipe.c)
