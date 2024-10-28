cmake_minimum_required(VERSION 3.5)

project("Vulkan Wrapper Demo")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)

include_directories(thirdparty/fmt/include)
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/fmt)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp" "src/*.h" "src/*.c")
add_executable("Demo1" ${SOURCES})
target_link_libraries(Demo1 PRIVATE glfw fmt)