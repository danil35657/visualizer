cmake_minimum_required(VERSION 3.10)

project(Visualizer CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS character_type.proto)

set(VISUALIZER_FILES main.cpp visualizer.h visualizer.cpp character_type.proto)

add_executable(visualizer ${PROTO_SRCS} ${PROTO_HDRS} ${VISUALIZER_FILES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT visualizer)
target_include_directories(visualizer PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(visualizer PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(visualizer "$<IF:$<CONFIG:Debug>,${Protobuf_LIBRARY_DEBUG},${Protobuf_LIBRARY}>" Threads::Threads)