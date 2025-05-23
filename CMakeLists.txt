# CMakeList.txt : CMake project for minecraft-LowLvReplica, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.15)
project("minecraft-LowLvReplica")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Source files
file(GLOB SRC src/*.cpp)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} ${SRC} ${GLAD_SRC})
