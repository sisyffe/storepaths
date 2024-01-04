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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.impl.osx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.impl.windows.cpp
)

# Compile options
if(CYGWIN)
    target_compile_options(libcfgpath
            PUBLIC
            /Wall
            /WX
    )
elseif(UNIX)
    target_compile_options(libcfgpath
            PUBLIC
            -Wall
            -Wpedantic
            -Wextra
            -Werror
    )
endif()
