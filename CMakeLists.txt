cmake_minimum_required(VERSION 4.3)
project(cpu)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
add_link_options(-fsanitize=address)
add_executable(cpu 
    Main.c
    Cpu.c
    Cpu.h
)
