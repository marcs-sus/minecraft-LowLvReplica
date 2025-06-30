# CMakeLists.txt for Minecraft LowLvReplica project

cmake_minimum_required(VERSION 3.15)
project("minecraft-LowLvReplica")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set libraries and include directories
# GLFW
set(GLFW3_ROOT_DIR "${CMAKE_SOURCE_DIR}/third-party/glfw")
find_package(GLFW3 REQUIRED)

# GLAD
set(GLAD_ROOT "${CMAKE_SOURCE_DIR}/third-party/glad")
include_directories(${GLAD_ROOT}/include)
file(GLOB GLAD_SRC ${GLAD_ROOT}/src/glad.c)

# GLM
set(GLM_ROOT "${CMAKE_SOURCE_DIR}/third-party/glm")
include_directories(${GLM_ROOT})

# STB
set(STB_ROOT "${CMAKE_SOURCE_DIR}/third-party/stb")
include_directories(${STB_ROOT})

# Source files
file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add source to this project's executable.
add_executable(${PROJECT_NAME} ${SRC} ${GLAD_SRC})

# Copy shaders to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/shaders"
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

# Copy assets to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/assets"
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${GLFW3_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  ${GLFW3_LIBRARY}
  opengl32.lib
)
