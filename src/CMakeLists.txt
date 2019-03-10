

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

set (APP_SHADER_FILES
    shader/display_vs.glsl
    shader/display_fs.glsl
    shader/compute.glsl
)
source_group( "Shaders"
    FILES ${APP_SHADER_FILES}
)

add_executable(raytrace-in-a-weekend WIN32
    ${APP_HEADER_FILES}
    ${APP_SOURCE_FILES}
    ${APP_SHADER_FILES}
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

EnableParallelCompile(raytrace-in-a-weekend)


add_custom_command(OUTPUT copyShader
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/src/shader" "$<TARGET_FILE_DIR:raytrace-in-a-weekend>/shader"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/src/shader" "$<TARGET_FILE_DIR:raytrace-in-a-weekend>/../shader"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Copying shader"
)

add_custom_command(TARGET raytrace-in-a-weekend POST_BUILD
    DEPENDS copyShader
    COMMENT "Copying shader to output directory"
)

add_custom_target(COPY_SHADERS DEPENDS copyShader)

if(EXISTS ${EXTERNALS_SOURCE})
    add_custom_command(TARGET raytrace-in-a-weekend POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glew> $<TARGET_FILE_DIR:raytrace-in-a-weekend>/$<TARGET_FILE_NAME:glew>
        COMMENT "Copying dependencies to output directory"
    )
endif()
