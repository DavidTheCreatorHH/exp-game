cmake_minimum_required(VERSION 3.10)
project(Experience)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only grab your actual source files
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "src/*.cpp")

add_executable(exp ${SRCS})

# Include headers
target_include_directories(exp PRIVATE
    include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
    ${PROJECT_SOURCE_DIR}/lib/glew/include
)

# OpenGL
find_package(OpenGL REQUIRED)

# Link EVERYTHING explicitly
target_link_libraries(exp
    OpenGL::GL
    ${PROJECT_SOURCE_DIR}/lib/glfw/libglfw.so
    ${PROJECT_SOURCE_DIR}/lib/glew/libGLEW.so
)

