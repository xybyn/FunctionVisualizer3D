cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(FunctionVisualizer3D)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.cpp

        )
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp
	)

file(GLOB_RECURSE SHADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.vert
        ${CMAKE_SOURCE_DIR}/src/*.frag
	)

configure_file(src/RootDir.h.in src/RootDir.h)
# Configure assets header file

include_directories(${CMAKE_BINARY_DIR}/src)


# IMGUI
file(GLOB_RECURSE IMGUI_FILES ${CMAKE_SOURCE_DIR}/include/imgui/*)

# STB_IMAGE
file(GLOB_RECURSE STB_IMAGE_FILES ${CMAKE_SOURCE_DIR}/include/stb_image/*)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES} ${IMGUI_FILES} ${STB_IMAGE_FILES} src/common/primitives/Point.cpp src/common/primitives/Point.h src/common/Utils.h src/common/shaders/NormalsShader.cpp src/common/shaders/NormalsShader.h)

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")



#######################################
# LOOK for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# ASSIMP
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${assimp_INCLUDE_DIRS}")


# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})


set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_DL_LIBS} ${assimp_LIBRARIES} GLEW)
# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)



# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS}) 
