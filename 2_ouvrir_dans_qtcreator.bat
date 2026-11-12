@echo off
rem Met Qt et MinGW dans le PATH pour que Qt Creator detecte le kit automatiquement
set PATH=C:\Qt\6.8.3\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%
start "" "C:\Qt\Tools\QtCreator\bin\qtcreator.exe" "%~dp0FireStationManager.pro"
