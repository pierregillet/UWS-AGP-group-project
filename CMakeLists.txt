cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

project(agp_group_project)
set(srcs main.cpp
        assetsLoader.cpp assetsLoader.h
        assetsPaths.h
        constants.h
        Mesh.cpp Mesh.h
        Game.cpp Game.h
        Light.cpp Light.h
        md2model.cpp md2model.h
        MotionBlurEffect.cpp MotionBlurEffect.h
        Player.cpp Player.h
        rt3d.cpp rt3d.h
        rt3dObjLoader.cpp rt3dObjLoader.h
        Shader.h Shader.cpp)
add_executable(agp_group_project ${srcs})

INCLUDE(FindPkgConfig)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

target_link_libraries(
        agp_group_project
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
)

include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2IMAGE_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
)
