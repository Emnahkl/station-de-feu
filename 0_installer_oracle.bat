@echo off
net session >nul 2>&1
if %errorlevel% neq 0 (
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)
title Installation Oracle XE 21c (silencieuse)
set SRC=%USERPROFILE%\Downloads\OracleXE213_Win64
set RSP=%~dp0XEInstall_auto.rsp
set LOG=%~dp0oracle_install_log.txt

if not exist "%SRC%\setup.exe" (
    echo setup.exe introuvable dans %SRC%
    pause
    exit /b
)

(
echo INSTALLDIR=C:\app\oracle\product\21c\
echo PASSWORD=esprit2026
echo LISTENER_PORT=1521
echo EMEXPRESS_PORT=5550
echo CHAR_SET=AL32UTF8
echo DB_DOMAIN=
) > "%RSP%"

echo ============================================================
echo  Installation d'Oracle XE en cours : 15 a 30 minutes.
echo  NE FERME PAS cette fenetre. Rien ne s'affiche, c'est normal.
echo ============================================================
start "" /wait "%SRC%\setup.exe" /s /v"RSP_FILE=\"%RSP%\"" /v"/L*v \"%LOG%\"" /v"/qn"
echo Code retour setup : %errorlevel%

del "%RSP%" >nul 2>&1
echo.
echo ===== Services Oracle =====
sc query state= all | findstr /I /C:"Oracle"
echo.
echo ===== FINI : dis a Claude "c'est fait" =====
pause
