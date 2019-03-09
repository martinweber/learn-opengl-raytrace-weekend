#

# Try to find GLFW library and include path.
# Once done this will define
#
# GLFW_FOUND
# GLFW_INCLUDE_DIR
# GLFW_LIBRARY
#

#set(GLFW_LOCATION ${GLFW_LOCATION} "${PROJECT_SOURCE_DIR}/externals/glfw-3.2")

find_path( 
    GLFW_LOCATION
	"include/GLFW/glfw3.h"
    PATHS 
		${COMMON_SDK_PATHS} 
    PATH_SUFFIXES 
        ${GLFW_SEARCH_SUFFIXES}
        "glfw"
)

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( GLFW_INCLUDE_DIR
        NAMES
            GLFW/glfw3.h
        PATHS
            ${GLFW_LOCATION}/include
            $ENV{GLFW_LOCATION}/include
            $ENV{PROGRAMFILES}/GLFW/include
            ${GLFW_LOCATION}
            $ENV{GLFW_LOCATION}
            DOC "The directory where GLFW/glfw3.h resides" )
    if(ARCH STREQUAL "x86")
      find_library( GLFW_LIBRARY
          NAMES
              glfw3
          PATHS
              ${GLFW_LOCATION}/lib
              $ENV{GLFW_LOCATION}/lib
              $ENV{PROGRAMFILES}/GLFW/lib
              DOC "The GLFW library")
    else()
      find_library( GLFW_LIBRARY
          NAMES
              glfw3
          PATHS
              ${GLFW_LOCATION}/lib
              $ENV{GLFW_LOCATION}/lib
              $ENV{PROGRAMFILES}/GLFW/lib
              ${GLFW_LOCATION}/lib/RelWithDebInfo
              DOC "The GLFW library")
    endif()
endif ()

if (${CMAKE_HOST_UNIX})
    find_path( GLFW_INCLUDE_DIR
        NAMES
            GLFW/glfw3.h
        PATHS
            ${GLFW_LOCATION}/include
            $ENV{GLFW_LOCATION}/include
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
            NO_DEFAULT_PATH
            DOC "The directory where GLFW/glfw3.h resides"
    )
    find_library( GLFW_LIBRARY
        NAMES
            glfw3 glfw
        PATHS
            ${GLFW_LOCATION}/lib
            $ENV{GLFW_LOCATION}/lib
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
            /usr/lib/x86_64-linux-gnu
            NO_DEFAULT_PATH
            DOC "The GLFW library")
endif ()

find_package_handle_standard_args(GLFW DEFAULT_MSG
    GLFW_INCLUDE_DIR
    GLFW_LIBRARY
)

if(GLFW_INCLUDE_DIR AND GLFW_LIBRARY)
    add_library(glfw INTERFACE)
    target_include_directories(glfw INTERFACE ${GLFW_INCLUDE_DIR})
    target_link_libraries(glfw INTERFACE ${GLFW_LIBRARY})
endif()

mark_as_advanced( GLFW_FOUND )