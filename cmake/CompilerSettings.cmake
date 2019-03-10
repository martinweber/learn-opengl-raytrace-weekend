
macro(ApplyCompilerSettingsGeneral)
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(/EHsc)                  # use exceptions
    add_definitions(/GR) 	                # use RTTI
    
    add_definitions(/W3)                    # warning level 3
    add_definitions(/GS)                    # enable security checks (stack overflow, buffer overruns etc.)
    add_definitions(/fp:fast)               # fast floating point model
endif()
endmacro()

macro(ApplyCompilerSettingsBuildTypes)
if(WIN32)
    # Configuration dependent settings
    # statically linked runtime
    set(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG}			/MTd")
    set(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE}			/MT")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO}	/MT")
endif()
endmacro()

macro(ApplyLinkerSettings)
    set( CMAKE_EXE_LINKER_FLAGS_DEBUG ${CMAKE_EXE_LINKER_FLAGS}  " /SUBSYSTEM:CONSOLE /NXCOMPAT:NO /DYNAMICBASE:NO /NODEFAULTLIB:LIBCMT /DEBUG" )
    set( CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO ${CMAKE_EXE_LINKER_FLAGS} " /SUBSYSTEM:CONSOLE /NXCOMPAT:NO /LTCG /DYNAMICBASE:NO /OPT:REF /DEBUG" )
    set( CMAKE_EXE_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS} " /SUBSYSTEM:CONSOLE /NXCOMPAT:NO /LTCG /DYNAMICBASE:NO /OPT:REF")
endmacro()

macro(EnableParallelCompile target)
	if (MSVC)
		target_compile_options(${target} PRIVATE "/MP" "/Gm-")
	endif()
endmacro()