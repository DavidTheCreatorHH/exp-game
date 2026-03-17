cmake_minimum_required(VERSION 3.10)
project("Experience")

# Set the executable name
set(EXECFILE "exp")

# Set the C++ standard
set(CXX_STD 17)

# Set the C++ standard in CMake
set(CMAKE_CXX_STANDARD ${CXX_STD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all .cpp files in the current directory and subdirectories
file(GLOB_RECURSE SRCS "*.cpp")
list(FILTER SRCS EXCLUDE REGEX "binary_to_compressed_c.cpp")
list(FILTER SRCS EXCLUDE REGEX "CompilerIdCXX/CMakeCXXCompilerId.cpp")

# Specify include directories
include_directories(include)

find_package(OpenGL REQUIRED)


# Coz CMake is evil
set(CMAKE_EXCLUDE_FROM_ALL ON)

# Add the executable
add_executable(${EXECFILE} ${SRCS})

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/")

target_link_libraries(${EXECFILE} 
    ${OPENGL_LIBRARIES} 
    glfw 
    GLEW 
    GLU 
    X11
)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/")

set_target_properties(${PRROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXECFILE})
