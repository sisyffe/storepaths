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

# Choose implementation file
if(WIN32)
    set(IMPLEMENTATION_FILE "cfgpath.impl.windows.cpp")
elseif(APPLE)
    set(IMPLEMENTATION_FILE "cfgpath.impl.osx.cpp")
elseif(UNIX)
    set(IMPLEMENTATION_FILE "cfgpath.impl.linux.cpp")
else()
    message(SEND_ERROR "Unknown platform")
endif()

target_sources(libcfgpath PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cfgpath.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${IMPLEMENTATION_FILE}
)

# Compile options
if(UNIX)
    target_compile_options(libcfgpath
            PUBLIC
            -Wall
            -Wpedantic
            -Wextra
            -Werror
    )
# Windows raises lots of unnecessary warnings if compiled with /Wall
# so default compilation
endif()
