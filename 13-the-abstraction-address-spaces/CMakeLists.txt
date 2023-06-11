cmake_minimum_required(VERSION 3.1)

project(
  ostep-ch-13
  VERSION 1.0
  LANGUAGES C)

option(ENABLE_ASAN "enable address sanitizer" On)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wconversion)

if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address -fno-omit-frame-pointer)
endif()

add_executable(memory-user memory-user.c)
