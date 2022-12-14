
# CMake entry point.
# Note that this CMakeLists.txt in fully independent from the one in the parent directory
# (since that's the goal of this tutorial !)

# For an introduction to CMake, see
# http://www.cmake.org/cmake/help/cmake_tutorial.html (at least steps 1 and 2)

cmake_minimum_required (VERSION 2.6)
project (zpg)

find_package(OpenGL REQUIRED)

# Ask the user to give the paths to GLFW, GLEW and GLM.
# On Linux this can be found automatically with pkg-config; 
# see http://stackoverflow.com/questions/6889264/cmake-flags-for-opengl-using-glew-and-glfw, for instance.

set(GLFW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glfw.h" )
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib" )

set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h" )
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib" )

set(GLM_INCLUDE_PATH "" CACHE PATH "The directory that contains glm/glm.hpp" )

include_directories(
        ${GLFW_INCLUDE_PATH}
        ${GLEW_INCLUDE_PATH}
        ${GLM_INCLUDE_PATH}
        /usr/include/assimp
)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(zpg
        source/glad.c
        source/main.cpp
        source/Shader.cpp
        source/Model.cpp
        source/DrawableObject.cpp source/Scene.cpp source/Transformation.cpp source/Observer.cpp source/Camera.cpp source/ShaderLoader.cpp source/Texture.cpp source/Skybox.cpp source/Animation.cpp)
target_link_libraries(zpg
        ${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
        ${GLFW_LIBRARY}
        glfw3
        dl
        pthread
        ${GLEW_LIBRARY}
        assimp
        )

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

target_include_directories(zpg PRIVATE ${CMAKE_SOURCE_DIR}/headers)
set_property(TARGET zpg PROPERTY CXX_STANDARD 23)



# HOW-TO. You can safely remove anything below.  

# test if linux, mac or windows : 

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message("You're on Mac OS !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message("You're on Linux !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message("You're on Windows !")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Yes this is weird but you have to repeat the "if" here.


# Add a checkbox in CMakeGUI : 

set(SOME_STUFF OFF CACHE BOOL "Check this to print an extra message at the beginning of main()")

# Add a preprocessor define : 

if(SOME_STUFF)
    add_definitions(
            -DCOMPILE_WITH_SOME_PREPROCESSOR_DIRECTIVE
    )
endif(SOME_STUFF)

add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Shaders copied to out dir")

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/assets
        COMMENT "Assets copied to out dir")

add_dependencies(zpg copy_shaders copy_assets)

#target_compile_options(zpg PRIVATE -fsanitize=address)
#target_link_options(zpg PRIVATE -fsanitize=address)