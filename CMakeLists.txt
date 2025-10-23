cmake_minimum_required(VERSION 3.10)
project(SimpleCMakeProject)

# Укажите версию C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Укажите папки с заголовками
include_directories(include)                     # Папка с заголовками
include_directories(include/imgui)               # Папка с заголовками ImGui
include_directories(include/imgui/backends)      # Папка с заголовками ImGui backends
include_directories(include/glm)


# Добавление источников
add_executable(SimpleCMakeProject 
    src/main.cpp 
    src/glad.c 
    include/imgui/imgui.cpp 
    include/imgui/imgui_draw.cpp 
    include/imgui/imgui_widgets.cpp 
    include/imgui/imgui_demo.cpp 
    include/imgui/backends/imgui_impl_glfw.cpp 
    include/imgui/backends/imgui_impl_opengl3.cpp)

# Связывание библиотеки GLFW и OpenGL
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(SimpleCMakeProject glfw OpenGL::GL)
