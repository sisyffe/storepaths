cmake_minimum_required(VERSION 3.20)

# Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Target
project(libcfgpath)
add_library(libcfgpath STATIC)
target_include_directories(libcfgpath PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(libcfgpath PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.impl.osx.cpp
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
