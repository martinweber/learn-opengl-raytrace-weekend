#.rst:
# FindGLEW
# --------
#
# Find the OpenGL Extension Wrangler Library (GLEW)
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` target ``GLEW::GLEW``,
# if GLEW has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   GLEW_INCLUDE_DIRS - include directories for GLEW
#   GLEW_LIBRARIES - libraries to link against GLEW
#   GLEW_FOUND - true if GLEW has been found and can be used

#=============================================================================
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

include(FindPackageHandleStandardArgs)

find_path(
    GLEW_INCLUDE_DIR 
		GL/glew.h 
    PATHS 
		${COMMON_SDK_PATHS} 
    PATH_SUFFIXES 
		"${GLEW_SEARCH_SUFFIXES}"
		"${GLEW_SEARCH_SUFFIXES}/include"
)

find_library(
    GLEW_LIBRARY 
    NAMES 
        GLEW glew32 glew glew32s 
    PATHS 
		${COMMON_SDK_PATHS} 
    PATH_SUFFIXES 
        lib64 
        "${GLEW_SEARCH_SUFFIXES}/lib/RelWithDebInfo"
        "${GLEW_SEARCH_SUFFIXES}/lib/Debug/x64"
        "${GLEW_SEARCH_SUFFIXES}/lib/Release/x64"
)

find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_INCLUDE_DIR GLEW_LIBRARY)

if(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    add_library(glew SHARED IMPORTED)
    target_include_directories(glew INTERFACE ${GLEW_INCLUDE_DIR})
    #target_link_libraries(glew INTERFACE ${GLEW_LIBRARY})
    set_property(TARGET glew PROPERTY IMPORTED_LOCATION ${GLEW_INCLUDE_DIR}/../bin/Release/x64/glew32.dll)
    set_property(TARGET glew PROPERTY IMPORTED_IMPLIB ${GLEW_LIBRARY})
endif()

mark_as_advanced(GLEW_FOUND)