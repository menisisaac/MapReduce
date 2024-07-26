cmake_minimum_required(VERSION 3.26.0)
project(MapReduce)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(rpclib REQUIRED)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(yaml-cpp GIT_REPOSITORY http://github.com/jbeder/yaml-cpp.git GIT_TAG master)
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
    message(STATUS "Fetching yaml-cpp...")
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()



add_executable(Coordinator src/main.cpp src/Coordinator.cpp include/Coordinator.h src/Scheduler.cpp include/Scheduler.h)
add_executable(Worker src/Worker.cpp include/Coordinator.h include/Plugin.h src/Coordinator.cpp src/Plugin.cpp src/Scheduler.cpp include/Scheduler.h)

target_link_libraries(Coordinator ${CMAKE_SOURCE_DIR}/rpclib/build/librpc.a yaml-cpp::yaml-cpp)
target_link_libraries(Worker ${CMAKE_SOURCE_DIR}/rpclib/build/librpc.a yaml-cpp::yaml-cpp nlohmann_json::nlohmann_json)

set_property(TARGET Coordinator PROPERTY CXX_STANDARD 23)
set_property(TARGET Worker PROPERTY CXX_STANDARD 23)
