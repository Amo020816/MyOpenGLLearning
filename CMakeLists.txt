cmake_minimum_required(VERSION 3.22)
project(Project_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Shaders src/Shaders/Shader.cpp
            src/Shaders/ShaderProgram.cpp)

set(MyIncludes  include/Shader
                include)



add_executable(${PROJECT_NAME}  src/Ch4_CoordinateSystem/GoingTo3D.cpp
                                ${Shaders}
                                vendor/glad.c
        )


add_subdirectory(vendor/glm)
add_subdirectory(vendor/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

include_directories(vendor)
include_directories(include)
include_directories(vendor/glfw/include/)