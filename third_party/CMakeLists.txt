cmake_minimum_required(VERSION 3.28)

#### glad
project(glad)
add_library(
        glad
        glad/src/gl.c
        
)

target_include_directories(glad PUBLIC glad/include)
target_compile_features(glad PUBLIC cxx_std_20)

#### glfw3
# https://www.glfw.org/docs/3.3/build_guide.html#build_link_cmake_source
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

#### GLM
add_subdirectory(glm)

#### Imgui
# add_subdirectory(imgui)
project(imgui)

# NOTE: I did not include all the backends here, only the ones I will be using
add_library(
        imgui
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC imgui imgui/backends)
target_link_libraries(
        imgui
        PRIVATE
        glfw
)
target_compile_features(imgui PUBLIC cxx_std_20)