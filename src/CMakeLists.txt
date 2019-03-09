

set( APP_HEADER_FILES
    StandardOutSink.h
    RaytraceApp.h
    helper/TinyShader.h
    helper/TinyTexture.h
    helper/glHelper.h
)

set( APP_SOURCE_FILES
    main.cpp
    RaytraceApp.cpp
    helper/TinyShader.cpp
    helper/TinyTexture.cpp
    helper/glHelper.cpp
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


if(EXISTS ${EXTERNALS_SOURCE})
    add_custom_command(TARGET raytrace-in-a-weekend POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glew> $<TARGET_FILE_DIR:raytrace-in-a-weekend>/$<TARGET_FILE_NAME:glew>
        COMMENT "Copying dependencies to output directory"
    )
endif()
