# - Try to find GLFW3
# Once done, this will define
#   GLFW3_FOUND - System has GLFW3
#   GLFW3_INCLUDE_DIR - The GLFW3 include directory
#   GLFW3_LIBRARY - The GLFW3 library

find_path(GLFW3_INCLUDE_DIR
    NAMES GLFW/glfw3.h
    PATHS
    ${GLFW3_ROOT_DIR}/include
    ENV GLFW3_ROOT_DIR
    ${CMAKE_SOURCE_DIR}/libs/glfw/include
    $ENV{PROGRAMFILES}/GLFW/include
    /usr/local/include
    /usr/include
)

find_library(GLFW3_LIBRARY
    NAMES glfw3
    PATHS
    ${GLFW3_ROOT_DIR}/lib-vc2022
    ENV GLFW3_ROOT_DIR
    ${CMAKE_SOURCE_DIR}/libs/glfw/lib-vc2022
    $ENV{PROGRAMFILES}/GLFW/lib-vc2022
    /usr/local/lib
    /usr/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)
