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
    ${CORE_DIR}/component.cpp
#    ${CORE_DIR}/system.cpp
    ${CORE_RESOURCE_MANAGER_DIR}/shader.cpp
    ${CORE_RESOURCE_MANAGER_DIR}/texture.cpp
    ${CORE_RESOURCE_MANAGER_DIR}/resourceManager.cpp
)


add_subdirectory(src/vendor)

set(PISTACHIO_INCLUDE_DIRS
    ${PISTACHIO_DIR}
#    ${APPLICATION_DIR}
    ${CORE_DIR}
    ${CORE_MODEL_DIR}
)

set(PISTACHIO_SOURCES
#    ${APPLICATION_SOURCES}
    ${CORE_SOURCES}
)

list(APPEND PISTACHIO_SOURCES ${PISTACHIO_DIR}/main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenMP REQUIRED)


add_executable(${CMAKE_PROJECT_NAME} ${PISTACHIO_SOURCES}
        src/Pistachio/core/spriteBatch.h
        src/Pistachio/core/spriteBatch.cpp
        src/Pistachio/core/spriteBatch.cpp
        src/Pistachio/core/spriteBatch.h
        src/Pistachio/common.h
        src/Pistachio/core/ecs/system/systemManager.h)
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${PISTACHIO_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PISTACHIO_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC vendor OpenGL::GL GLEW::GLEW)


list(REMOVE_ITEM PISTACHIO_SOURCES ${PISTACHIO_DIR}/main.cpp)
list(APPEND PISTACHIO_SOURCES src/test/test.cpp)

add_executable(${CMAKE_PROJECT_NAME}_test ${PISTACHIO_SOURCES})
target_sources(${CMAKE_PROJECT_NAME}_test PUBLIC ${PISTACHIO_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME}_test PUBLIC ${PISTACHIO_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME}_test PUBLIC vendor OpenGL::GL GLEW::GLEW OpenMP::OpenMP_CXX)