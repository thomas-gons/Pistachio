cmake_minimum_required(VERSION 3.27)
project(Pistachio)

set(CMAKE_CXX_STANDARD 17)

set(PISTACHIO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Pistachio)


##################### Application #####################

set(APPLICATION_DIR ${PISTACHIO_DIR}/application)

set(APPLICATION_SOURCES
    ${APPLICATION_DIR}/application.cpp
)


##################### Core #####################

set(CORE_DIR ${PISTACHIO_DIR}/core)
set(CORE_MODEL_DIR ${CORE_DIR}/model)
set(CORE_RESOURCE_MANAGER_DIR ${CORE_DIR}/resourceManager)

set(CORE_SOURCES
    ${CORE_RESOURCE_MANAGER_DIR}/shader.cpp
    ${CORE_RESOURCE_MANAGER_DIR}/texture.cpp
    ${CORE_RESOURCE_MANAGER_DIR}/resourceManager.cpp
)


add_subdirectory(src/vendor)

set(PISTACHIO_INCLUDE_DIRS
    ${PISTACHIO_DIR}
    ${APPLICATION_DIR}
    ${CORE_DIR}
    ${CORE_MODEL_DIR}
)

set(PISTACHIO_SOURCES
    ${APPLICATION_SOURCES}
    ${CORE_SOURCES}
)

list(APPEND PISTACHIO_SOURCES ${PISTACHIO_DIR}/main.cpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)


add_executable(${CMAKE_PROJECT_NAME} ${PISTACHIO_SOURCES})
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${PISTACHIO_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PISTACHIO_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC vendor SDL2::SDL2 SDL2_image OpenGL::GL GLEW::GLEW)