@echo off
set return=exit /b

if "%1" == "-a" (
  shift
) else (
  if "%1" == "" %return% -1
  type nul > "%1"
)
if "%1" == "" %return% -1
echo.---------- %date% %time%>>"%1"
for /f "tokens=2* delims=:" %%a in ('findstr /n "^"') do (
  echo.%%a
  echo.%%a>> "%1"
)
echo.>>"%1"
%return% 0
