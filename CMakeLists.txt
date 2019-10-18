
cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW) # require VERSION option for project command
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(raytrace-in-a-weekend VERSION 0.1)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include("cmake/CompilerSettings.cmake")
ApplyCompilerSettingsGeneral()
ApplyCompilerSettingsBuildTypes()
ApplyLinkerSettings()

set(COMMON_SDK_PATHS
    "libs"
    "externals"
    "${CMAKE_SOURCE_DIR}/externals"
    "${CMAKE_SOURCE_DIR}/../externals"
    "${CMAKE_SOURCE_DIR}/../../externals"
    "${CMAKE_SOURCE_DIR}/../../../externals"
    "${CMAKE_SOURCE_DIR}/../../../externals"
    "${CMAKE_SOURCE_DIR}/../../../../externals"
    "C:/dev/tachyon/externals"
)


find_package(G3LOG REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)


add_subdirectory(src)


