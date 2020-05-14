cmake_minimum_required (VERSION 2.6)
project (fixed_point_math_lib)

add_library(fixed_point STATIC src/fixed_point.c)
target_include_directories(fixed_point PUBLIC include)
target_compile_definitions(fixed_point PUBLIC FXP_C99_INLINE)