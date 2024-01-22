# CMakeLists.txt (src/vendor)


add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE imgui::imgui stb_image::stb_image)


### IM_GUI ###

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)
set(IMGUI_FREETYPE_DIR ${IMGUI_DIR}/misc/freetype)

#[[
    ImGui provides the following graphics APIs out of the box:
        - OpenGL 2 and 3
        - Vulkan
        - GLFW
        - DirectX 9, 10, 11, 12
        - ... and more (see src/vendor/imgui/backends)
]]

set(DEFAULT_GRAPHICS_API "opengl3")
set(GRAPHICS_API ${DEFAULT_GRAPHICS_API} CACHE STRING "Graphics API to use for ImGui")

# Make sure the graphics API is lowercase
string(TOLOWER ${GRAPHICS_API} GRAPHICS_API)

if (GRAPHICS_API STREQUAL "opengl2" OR GRAPHICS_API STREQUAL "opengl3")
    set(GRAPHICS_API_RAW "OpenGL")
elseif (GRAPHICS_API STREQUAL "vulkan")
    set(GRAPHICS_API_RAW "Vulkan")
else()
    message(FATAL_ERROR "Invalid graphics API: ${GRAPHICS_API}")

endif()

message("Using ${GRAPHICS_API} for ImGui")

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_BACKEND_DIR}/imgui_impl_${GRAPHICS_API}.cpp
        ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp

        ${IMGUI_FREETYPE_DIR}/imgui_freetype.cpp
)

find_package(SDL2 REQUIRED)
find_package(${GRAPHICS_API_RAW} REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


add_library(imgui)
target_sources(imgui PRIVATE ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_BACKEND_DIR} ${FREETYPE_INCLUDE_DIRS} ${IMGUI_FREETYPE_DIR} ${GLEW_INCLUDE_DIRS})
target_link_libraries(imgui PUBLIC SDL2::SDL2 ${GRAPHICS_API_RAW}::${GRAPHICS_API_RAW} ${FREETYPE_LIBRARIES} ${GLEW_LIBRARIES} glfw)

add_library(imgui::imgui ALIAS imgui)


### STB_IMAGE ###

set(STB_IMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
set(STB_IMAGE_SOURCES ${STB_IMAGE_DIR}/stb_image.cpp)

add_library(stb_image)
target_include_directories(stb_image PUBLIC ${STB_IMAGE_DIR})
target_sources(stb_image PRIVATE ${STB_IMAGE_SOURCES})

add_library(stb_image::stb_image ALIAS stb_image)
