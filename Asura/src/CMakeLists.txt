cmake_minimum_required(VERSION 3.14)

set(TARGET Asura)

include_directories(${PROJECT_SOURCE_DIR}/Asura/src/Asura)
include_directories(${PROJECT_SOURCE_DIR}/Asura/src)
include_directories(${PROJECT_SOURCE_DIR}/vendor/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor)
include_directories(${PROJECT_SOURCE_DIR}/vendor/imgui)
include_directories(${PROJECT_SOURCE_DIR}/vendor/Glad/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/Glad/include/glad)
include_directories(${PROJECT_SOURCE_DIR}/vendor/entt/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/YAML_CPP/include)
include_directories(D:/VulKanSDK/Include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/box2d/include)


file(GLOB_RECURSE LIB_SOURCES
        ${PROJECT_SOURCE_DIR}/Asura/src/*.cpp
        )

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/vendor/Asura)

MESSAGE(STATUS "lib sources" ${LIB_SOURCES})

add_library(${TARGET} SHARED ${LIB_SOURCES}
        ${PROJECT_SOURCE_DIR}/vendor/Glad/src/glad.c
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imconfig.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_internal.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
        ${PROJECT_SOURCE_DIR}/vendor/stb_image/stb_image.cpp
        )

target_link_libraries(${TARGET}
        ${PROJECT_SOURCE_DIR}/vendor/GLFW/lib-mingw-w64/glfw3.dll
        opengl32.dll
        ${PROJECT_SOURCE_DIR}/vendor/YAML_CPP/lib/libyaml-cpp.a

        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-core.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-cpp.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-glsl.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-hlsl.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-msl.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-reflect.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-util.a

        ${PROJECT_SOURCE_DIR}/vendor/shaderc/shaderc_shared.lib
        ${PROJECT_SOURCE_DIR}/vendor/shaderc/shaderc_shared.dll

        ${PROJECT_SOURCE_DIR}/vendor/box2d/lib/libbox2d.a
        )

# macros
ADD_DEFINITIONS(-DASURA_PLATFORM_WINDOWS -DASURA_DEBUG -DASURA_BUILD_DLL -DASURA_ENABLE_ASSERT
        -DYAML_CPP_STATIC_DEFINE)

