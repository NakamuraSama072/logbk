cmake_minimum_required(VERSION 3.10)
project(logbk VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    main.cpp
    src/utils.cpp
    src/bktool.cpp
    src/prjcli.cpp
)

add_executable(logbk ${SOURCES})

target_compile_options(logbk PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

install(TARGETS logbk DESTINATION bin)
