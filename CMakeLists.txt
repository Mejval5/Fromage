cmake_minimum_required(VERSION 3.30)
project(Fromage C)
set(CMAKE_C_STANDARD 23)

# Add these lines:
add_compile_options("$<$<C_COMPILER_ID:GNU>:-finput-charset=UTF-8>")
add_compile_options("$<$<C_COMPILER_ID:GNU>:-fexec-charset=UTF-8>")

# Keep the MSVC ones if you want cross-compiler compatibility, they won't hurt GCC
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(Fromage fromage.c
        le_morceau.c
        la_mémoire.h
        la_mémoire.c
        le_débogage.c
        le_débogage.h
        la_valeur.c
        la_valeur.h)
