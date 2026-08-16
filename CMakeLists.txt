cmake_minimum_required(VERSION 3.16)
project(Nocturne VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/core/process.cpp
    src/core/memory.cpp
    src/core/module.cpp
    src/core/pattern.cpp
    src/unity/unity_transform.cpp
    src/unity/unity_camera.cpp
    src/unity/unity_skeleton.cpp
    src/unity/unity_resolver.cpp
    src/registry/entity_registry.cpp
    src/registry/entity_scanner.cpp
    src/math/projection.cpp
    src/overlay/overlay.cpp
)

set(HEADERS
    src/core/process.h
    src/core/memory.h
    src/core/module.h
    src/core/pattern.h
    src/unity/unity_structs.h
    src/unity/unity_transform.h
    src/unity/unity_camera.h
    src/unity/unity_skeleton.h
    src/unity/unity_entity.h
    src/unity/unity_resolver.h
    src/registry/entity_registry.h
    src/registry/entity_scanner.h
    src/math/vec3.h
    src/math/matrix.h
    src/math/projection.h
    src/overlay/overlay.h
)

# Library
add_library(nocturne STATIC ${SOURCES} ${HEADERS})
target_include_directories(nocturne PUBLIC src)

if(WIN32)
    target_link_libraries(nocturne PUBLIC dwmapi d3d11)
endif()

# Demo executable
add_executable(NocturneDemo src/demo/demo.cpp)
target_link_libraries(NocturneDemo PRIVATE nocturne)

# Tests
add_executable(NocturneTests tests/test_runner.cpp)
target_link_libraries(NocturneTests PRIVATE nocturne)
