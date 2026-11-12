@echo off
net session >nul 2>&1
if %errorlevel% neq 0 (
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)
title Demarrage Oracle XE
set OUT=%~dp0diagnostic_oracle.txt
echo ===== DIAGNOSTIC ORACLE %date% %time% ===== > "%OUT%"

echo. >> "%OUT%"
echo ===== Services Oracle ===== >> "%OUT%"
sc query state= all | findstr /I /C:"Oracle" >> "%OUT%"

echo. >> "%OUT%"
echo ===== Dossiers d'installation ===== >> "%OUT%"
dir /b C:\app >> "%OUT%" 2>&1
where /r C:\app lsnrctl.exe >> "%OUT%" 2>&1
where /r C:\app listener.ora >> "%OUT%" 2>&1

echo. >> "%OUT%"
echo ===== Demarrage des services ===== >> "%OUT%"
for /f "tokens=2" %%s in ('sc query state^= all ^| findstr /I /C:"TNSListener"') do (
    sc config %%s start= auto >> "%OUT%" 2>&1
    net start %%s >> "%OUT%" 2>&1
)
sc config OracleServiceXE start= auto >> "%OUT%" 2>&1
net start OracleServiceXE >> "%OUT%" 2>&1

echo Attente 25 secondes...
timeout /t 25 /nobreak >nul

echo. >> "%OUT%"
echo ===== Port 1521 ===== >> "%OUT%"
netstat -ano | findstr ":1521" >> "%OUT%"

echo. >> "%OUT%"
echo ===== lsnrctl status ===== >> "%OUT%"
for /f "delims=" %%l in ('where /r C:\app lsnrctl.exe 2^>nul') do "%%l" status >> "%OUT%" 2>&1

echo. >> "%OUT%"
echo ===== listener.ora ===== >> "%OUT%"
for /f "delims=" %%f in ('where /r C:\app listener.ora 2^>nul') do type "%%f" >> "%OUT%"

echo. >> "%OUT%"
echo ===== Logs installation ===== >> "%OUT%"
dir /b /s "C:\Program Files\Oracle\Inventory\logs" >> "%OUT%" 2>&1

type "%OUT%"
echo.
echo ===== FINI : dis a Claude "c'est fait" =====
pause
