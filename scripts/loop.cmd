@echo off
set _number=0
:start
if %_number%==10 goto end

:: do something
echo %_number% 

set /a _number +=1
goto start
:end
