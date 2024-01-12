# CMakeLists.txt (src/vendor)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE imgui::imgui)


# IM_GUI
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)


set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_BACKEND_DIR}/imgui_impl_sdl2.cpp
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)


add_library(imgui)
target_sources(imgui PRIVATE ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_BACKEND_DIR})
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)

add_library(imgui::imgui ALIAS imgui)