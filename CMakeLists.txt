cmake_minimum_required(VERSION 3.17)
project(transport_stream_parser_rework)

set(CMAKE_CXX_STANDARD 14)

file(GLOB transport_stream_parser_rework_FILES
        "headers/*.h"
        "sources/*.cpp"
        "main.cpp"
        )

add_executable(transport_stream_parser_rework ${transport_stream_parser_rework_FILES})
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)