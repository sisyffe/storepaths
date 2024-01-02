cmake_minimum_required(VERSION 3.20)

# Standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Target
project(libcfgpath C)
add_library(libcfgpath STATIC)
target_include_directories(libcfgpath PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(libcfgpath PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.impl.macos.c
)

# # Use Cocoa if apple
# if (APPLE)
#     find_library(COCOA Cocoa)
#     if (NOT COCOA)
#         message(FATAL_ERROR "Cocoa not found")
#     endif()
#     target_link_libraries(libgrades PRIVATE ${COCOA})
# endif()

# Compile options
target_compile_options(libcfgpath
        PUBLIC
        -Wall
        -Wpedantic
        -Wextra
        -Werror
)
