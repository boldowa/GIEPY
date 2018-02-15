@echo off
cd /d "%~dp0"
setlocal enabledelayedexpansion
set return=exit /b

rem +---------------------------------------------+
rem | If you want to use option,                  |
rem | set here.                                   |
rem +---------------------------------------------+
set OPTION=-w -L routines -p







rem +---------------------------------------------+
rem | Main routine                                |
rem +---------------------------------------------+
title GIEPY

set RomPath=
set ListPath=
cls
echo.

rem execute check
call "%~dp0giepy" -v
set result=%ERRORLEVEL%
if %result% neq 0 (
	title GIEPY - SYSTEM ERROR
	pause
	%return% %result%
)
echo.
echo   Current options: %OPTION%
echo       (If you want to change options, please edit this batch file.)

if '%1' == '' (
	echo.
	echo Usage: %~n0 ^<ROM^> ^<LIST^>
) else (
	cd /d "%~dp1"
	set RomPath=%~1
	shift
)
if not '%1' == '' (
	set ListPath=%~1
)

echo.
if '"%RomPath%"' == '""' (
:romloop
	set RomPath=
	set /p RomPath=Enter ROM filename: 
	if '' == '!RomPath!' (
		echo Error: empty.
		pause
		%return% 1
	)
	if not exist "!RomPath!" (
		echo Error: "!RomPath!" not found.
		goto :romloop
	)
)

echo.
if '"%ListPath%"' == '""' (
:listloop
	set ListPath=
	set /p ListPath=Enter list filename: 
	if '' == '!ListPath!' (
		echo List empty. use the default value.
	) else (
		if not exist "!ListPath!" (
			echo Error: "!ListPath!" not found.
			goto :listloop
		)
	)
)

call :GiepyCaller %0 "%RomPath%" "%ListPath%"
set result=%ERRORLEVEL%
pause
%return% %result%



:GiepyCaller
cls
set lll=
if not '%~3' == '' (
	set lll=-l "%~3"
	set lll_shadow=-l "%~nx3"
)
echo.
echo giepy %OPTION% "%~nx2" %lll_shadow%
title GIEPY %OPTION% "%~nx2" %lll_shadow%
call "%~dp0giepy" %OPTION% "%~2" %lll%
%return% %ERRORLEVEL%
