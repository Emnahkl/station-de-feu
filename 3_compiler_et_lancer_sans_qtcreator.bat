@echo off
title Compilation FireStationManager
set PATH=C:\Qt\6.8.3\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%
taskkill /f /im FireStationManager.exe >nul 2>&1
timeout /t 1 /nobreak >nul
cd /d "%~dp0"
if not exist build mkdir build
cd build
qmake ..\FireStationManager.pro "CONFIG+=release" || goto erreur
mingw32-make -j4 || goto erreur
windeployqt release\FireStationManager.exe >nul
start "" release\FireStationManager.exe
exit /b

:erreur
echo ***** ERREUR de compilation : fais une capture d'ecran et envoie-la a Claude *****
pause
