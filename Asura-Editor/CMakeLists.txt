CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

include_directories(${PROJECT_SOURCE_DIR}/Asura/src/Asura)
include_directories(${PROJECT_SOURCE_DIR}/Asura/src)
include_directories(${PROJECT_SOURCE_DIR}/vendor/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor)
include_directories(${PROJECT_SOURCE_DIR}/vendor/imgui)
include_directories(${PROJECT_SOURCE_DIR}/vendor/Glad/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/Glad/include/glad)
include_directories(${PROJECT_SOURCE_DIR}/vendor/entt/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/yaml-cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/ImGuizmo)
include_directories(D:/VulKanSDK/Include)
include_directories(D:/VulKanSDK/Include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/box2d/include)



add_executable(Asura-Editor
        src/AsuraEditorApp.cpp
        src/EditorLayer.cpp
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
        ${PROJECT_SOURCE_DIR}/Asura/src/Asura/Debug/Instrumentor.cpp
        ${PROJECT_SOURCE_DIR}/Asura/src/Asura/Scene/Entity.cpp
        ${PROJECT_SOURCE_DIR}/Asura/src/Asura/Scene/Scene.cpp
        ../Asura/src/Asura/Scene/SceneCamera.cpp ../Asura/src/Asura/Scene/SceneCamera.h
        ../Asura/src/Asura/Scene/ScriptableEntity.cpp ../Asura/src/Asura/Scene/ScriptableEntity.h
        src/Panels/SceneHierarchyPanel.cpp src/Panels/SceneHierarchyPanel.h
        ${PROJECT_SOURCE_DIR}/Asura/src/Asura/Scene/SceneSerializer.cpp
        ../Asura/src/Asura/Utils/PlatformUtils.h ../Asura/src/Platform/Windows/WindowsPlatformUtils.cpp
        ../Asura/src/Asura/Math/Math.h ../Asura/src/Asura/Math/Math.cpp
        ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/ImGuizmo.h
        ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/ImGuizmo.cpp
        ../Asura/src/Asura/Renderer/EditorCamera.cpp ../Asura/src/Asura/Renderer/EditorCamera.h
        ../Asura/src/Asura/Core/Timer.cpp ../Asura/src/Asura/Core/Timer.h
        ../Asura/src/Asura/Renderer/UniformBuffer.cpp ../Asura/src/Asura/Renderer/UniformBuffer.h
        ../Asura/src/Platform/OpenGL/OpenGLUniformBuffer.cpp ../Asura/src/Platform/OpenGL/OpenGLUniformBuffer.h
        src/Panels/ContentBrowserPanel.cpp src/Panels/ContentBrowserPanel.h
        ../Asura/src/Asura/Core/GUID.cpp ../Asura/src/Asura/Core/GUID.h)

add_custom_command(TARGET Asura-Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/libAsura.dll.a"
        ${PROJECT_SOURCE_DIR}/build/Asura-Editor)
add_custom_command(TARGET Asura-Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/libAsura.dll"
        ${PROJECT_SOURCE_DIR}/build/Asura-Editor)


# link Asura dll
target_link_libraries(Asura-Editor
        E:/Asura/build/Asura-Editor/glfw3.dll
        ${ASURA}
        ${GLEW}
        opengl32.dll
        )

# macros
ADD_DEFINITIONS(-DASURA_PLATFORM_WINDOWS -DASURA_DEBUG -DASURA_ENABLE_ASSERT
        -DYAML_CPP_STATIC_DEFINE)
