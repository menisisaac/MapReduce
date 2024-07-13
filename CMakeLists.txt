cmake_minimum_required(VERSION 3.26.0)
project(MapReduce)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(rpclib REQUIRED)
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)


add_executable(Coordinator src/main.cpp src/Coordinator.cpp include/Coordinator.h)
add_executable(Worker src/Worker.cpp include/Coordinator.h include/Plugin.h src/Coordinator.cpp src/Plugin.cpp)

target_link_libraries(Coordinator ${CMAKE_SOURCE_DIR}/rpclib/build/librpc.a)
target_link_libraries(Worker ${CMAKE_SOURCE_DIR}/rpclib/build/librpc.a nlohmann_json::nlohmann_json)

set_property(TARGET Coordinator PROPERTY CXX_STANDARD 23)
set_property(TARGET Worker PROPERTY CXX_STANDARD 23)
