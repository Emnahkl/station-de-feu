@echo off
title Installation Qt 6.8.3 sans compte Qt
echo ============================================================
echo  Installation de Qt 6.8.3 MinGW 64-bit + Qt Charts + Qt Creator
echo  (sans compte Qt) dans C:\Qt  -  environ 1,5 Go, 10-20 min
echo ============================================================
echo.

set PY=
where py >nul 2>nul && set PY=py
if not defined PY where python >nul 2>nul && set PY=python
if not defined PY (
    echo Python introuvable : installation de Python 3.12 avec winget...
    winget install -e --id Python.Python.3.12 --accept-source-agreements --accept-package-agreements
    echo.
    echo Python est installe. FERME cette fenetre et relance 1_installer_qt.bat
    pause
    exit /b
)

echo Python trouve : %PY%
%PY% -m pip install --upgrade aqtinstall || goto erreur

%PY% -m aqt install-qt windows desktop 6.8.3 win64_mingw -m qtcharts -O C:\Qt || goto erreur
%PY% -m aqt install-tool windows desktop tools_mingw1310 qt.tools.win64_mingw1310 -O C:\Qt || goto erreur
%PY% -m aqt install-tool windows desktop tools_qtcreator_gui qt.tools.qtcreator_gui -O C:\Qt || goto erreur

echo.
echo ============================================================
echo  TERMINE ! Lance maintenant 2_ouvrir_dans_qtcreator.bat
echo ============================================================
pause
exit /b

:erreur
echo.
echo ***** ERREUR pendant l'installation : fais une capture d'ecran et envoie-la a Claude *****
pause
