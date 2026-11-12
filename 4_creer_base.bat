@echo off
title Creation de la base FIRESTATION
set SQLP=C:\app\oracle\product\21c\dbhomeXE\bin\sqlplus.exe
set OUT=%~dp0creation_base_log.txt
cd /d "%~dp0sql"
"%SQLP%" -L /nolog @setup_base.sql > "%OUT%" 2>&1
type "%OUT%"
echo.
echo ===== FINI : dis a Claude "c'est fait" =====
pause
