# CMakeLists.txt for opengl-cube project

cmake_minimum_required(VERSION 3.15)
project("opengl-cube")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find used packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Source files
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB GLAD_SRC third-party/glad/src/glad.c)

# Add source to this project's executable
add_executable(${PROJECT_NAME}
    ${SRC}
    ${GLAD_SRC}
)

# Link libraries and include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
        third-party/glad/include
        third-party/stb
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        OpenGL::GL
        glm::glm
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
