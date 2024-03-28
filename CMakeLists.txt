cmake_minimum_required(VERSION 3.20)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Target
project(storepaths)
if(NOT BUILD_SHARED_LIBS)
    add_library(storepaths STATIC)
else()
    add_library(storepaths SHARED)
    set_target_properties(storepaths PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            SOVERSION 1
    )
endif()
target_include_directories(storepaths PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Choose implementation file(s)
if(WIN32)
    set(IMPLEMENTATION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.windows.cpp")
    set(SECOND_IMPLEMENTATION_FILE "")
elseif(APPLE)
    set(IMPLEMENTATION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.osx.cpp")
    set(SECOND_IMPLEMENTATION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.posix.cpp")
elseif(UNIX)
    set(IMPLEMENTATION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.impl.posix.cpp")
    set(SECOND_IMPLEMENTATION_FILE "")
else()
    message(SEND_ERROR "Unknown platform")
endif()

target_sources(storepaths PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/storepaths.cpp
        ${IMPLEMENTATION_FILE}
        ${SECOND_IMPLEMENTATION_FILE}
)

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
