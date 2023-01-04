cmake_minimum_required(VERSION 3.18.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -funwind-tables")

project("native3d")

# Add glm library
add_subdirectory("../../../thirdparty/glm" ${CMAKE_CURRENT_BINARY_DIR}/glm)
message("glm dir : ${glm_SOURCE_DIR}")
include_directories("${glm_SOURCE_DIR}")

# Add stb_image library
include_directories("../../../thirdparty/stb")

# Addassimp library
set(ASSIMP_SRC "../../../thirdparty/assimp")
add_subdirectory(${ASSIMP_SRC} ${CMAKE_CURRENT_BINARY_DIR}/assimp)

include_directories("./engine")

file(GLOB native3d_SRC
        "./*.cpp")

add_library(native3d SHARED ${native3d_SRC})

find_library(log-lib log)

target_link_libraries(
        native3d
        GLESv3
        android
        EGL
        atomic
        jnigraphics
        glm
        assimp
        ${log-lib})