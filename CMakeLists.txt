cmake_minimum_required(VERSION 3.22)
project(Minecraft_clone)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Shaders src/Shaders/Shader.cpp
            src/Shaders/ShaderProgram.cpp)

set(Camera src/Camera/Camera.cpp)

add_executable(${PROJECT_NAME}  src/main/Ch5_Lighting/GoingTo3D.cpp
                                ${Shaders}
                                vendor/glad.c
                                ${Camera}
        )

add_subdirectory(vendor/glm)
add_subdirectory(vendor/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

include_directories(vendor)
include_directories(include)
include_directories(vendor/glfw/include/)