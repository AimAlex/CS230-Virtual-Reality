# CMake entry point
cmake_minimum_required (VERSION 3.7)
project (VR_HW)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies 
# add_subdirectory (external_dependencies)

include_directories(
    external_dependencies/include/
	{SDL2_INCLUDE_DIRS}
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    freeimage
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# Tutorial 1
add_executable(hw1
	./main.cpp
    ./sdlWrapper.cpp
    ./shader.cpp
    ./utils.cpp
)
target_link_libraries(hw1
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(hw1 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/hw1/")