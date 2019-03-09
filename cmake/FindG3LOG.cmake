#
# Find G3LOG
#
#
#

include(FindPackageHandleStandardArgs)

find_path( 
    G3LOG_INCLUDE_DIR
    NAMES 
        g3log/g3log.hpp
    PATHS
        ${COMMON_SDK_PATHS}
    PATH_SUFFIXES 
        "include"
        "g3log/include"
        "${G3LOG_SEARCH_SUFFIXES}/include"
    DOC "The directory where g3log/g3log.hpp resides"
)

if(WIN32)
    find_library( G3LOG_LIBRARY_DEBUG
        NAMES
            g3logger.lib
        PATHS 
            "${CMAKE_SOURCE_DIR}/externals/${G3LOG_DIRNAME}"
            ${COMMON_SDK_PATHS}
        PATH_SUFFIXES 
            "lib/debug"
            "g3log/lib/debug"
            "${G3LOG_SEARCH_SUFFIXES}/lib/debug"
        DOC "The G3Log library -debug"
    )
	
	find_library( G3LOG_LIBRARY_RELEASE
        NAMES
            g3logger.lib
        PATHS 
            "${CMAKE_SOURCE_DIR}/externals/${G3LOG_DIRNAME}"
            ${COMMON_SDK_PATHS}
        PATH_SUFFIXES 
            "lib/release"
            "g3log/lib/release"
            "${G3LOG_SEARCH_SUFFIXES}/lib/release"
        DOC "The G3Log library -release"
    )
    
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(G3LOG DEFAULT_MSG G3LOG_INCLUDE_DIR G3LOG_LIBRARY_DEBUG)

if(G3LOG_INCLUDE_DIR AND G3LOG_LIBRARY_DEBUG AND G3LOG_LIBRARY_RELEASE)
    add_library(g3log INTERFACE)
    target_include_directories(g3log INTERFACE ${G3LOG_INCLUDE_DIR})
    target_link_libraries(g3log INTERFACE
        debug ${G3LOG_LIBRARY_DEBUG}
        optimized ${G3LOG_LIBRARY_RELEASE})
endif()

# Hide some variables
mark_as_advanced(G3LOG_INCLUDE_DIR)

