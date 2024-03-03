cmake_minimum_required(VERSION 3.25)

project(
    LearnWebGPU
    VERSION 0.1.0
    LANGUAGES CXX C
)

add_executable(App main.cpp)

add_subdirectory(glfw)
target_link_libraries(App PRIVATE glfw)

set_target_properties(App PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)
target_compile_options(App PRIVATE -Wall -Wextra -pedantic)

