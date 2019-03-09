@echo off
if "%1"=="" goto usage

SET CMAKE_GENERATOR="Visual Studio 15 2017 Win64"

if "%1" == "clean" set doClean=1
if "%1" == "clean-all" set doClean=1

if defined doClean (
	if not exist "build" (
	        mkdir build
	)
	cd build
	
	if exist "CmakeCache.txt" (
	 	del CMakeCache.txt
	)
	
	cd ..
)

if "%1" == "clean-all" (
	cd libs
	call buildlibs.bat clean %2 %3 %4 %5 %6 %7 %8 %9
	cd ..
	goto eof
)

if "%1" == "clean" (
	goto eof
)

if "%1" == "nuke" (
	del /s /q build
	del /s /q setup\files
	goto eof
)

if "%1" == "nuke-all" (
	del /s /q build
	del /s /q setup\files
	cd libs
	call buildlibs.bat nuke-all %2 %3 %4 %5 %6 %7 %8 %9
	cd ..
	goto eof
)

if "%1" == "configure" (
	if not exist "build" (
        mkdir build
	)

	cd build
	cmake %2 %3 %4 %5 %6 %7 %8 %9 .. -G %CMAKE_GENERATOR%
	cd ..
	goto eof
)


:usage
@echo Usage: %0 [clean^|nuke^|nuke-all^|configure]
exit /B 1

:eof
SET CMAKE_GENERATOR=
