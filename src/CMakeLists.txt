

set( APP_HEADER_FILES
    StandardOutSink.h
    RaytraceApp.h
)

set( APP_SOURCE_FILES
    main.cpp
    RaytraceApp.cpp
)

add_executable(raytrace-in-a-weekend WIN32
    ${APP_HEADER_FILES}
    ${APP_SOURCE_FILES}
)

target_link_libraries(raytrace-in-a-weekend PUBLIC
    glfw
    g3log
    glm
    glew
    opengl32

    # imgui
    # Vulkan
)
