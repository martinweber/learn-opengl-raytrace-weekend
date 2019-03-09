@echo off

FOR /D %%i IN (*.*) DO (
	cd "%%i"
	if exist "build.bat" call "build.bat" %1 %2 %3 %4 %5 %6 %7 %8 %9
	cd ..
)