cmake_minimum_required(VERSION 3.20)

## Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

## Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libgrades
project(storepaths)
## target
if(NOT BUILD_SHARED_LIBS)
    add_library(storepaths STATIC)
else()
    add_library(storepaths SHARED)
    set_target_properties(storepaths PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            SOVERSION 1
    )
endif()
## sources
target_include_directories(storepaths PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(storepaths PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.cpp
)
### Choose implementation file(s)
if(WIN32)
    target_sources(storepaths PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.windows.cpp")
elseif(APPLE)
    target_sources(storepaths PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.osx.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.posix.cpp"
    )
elseif(UNIX)
    target_sources(storepaths PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.posix.cpp")
else()
    message(SEND_ERROR "Unknown platform")
endif()

# Compile options
if(UNIX)
    target_compile_options(storepaths
            PUBLIC
            -Wall
            -Wpedantic
            -Wextra
            -Werror
    )
# Windows raises lots of unnecessary warnings if compiled with /Wall
# so default compilation
endif()
