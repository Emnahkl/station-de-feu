@echo off
title Push GitHub - branche campagne
cd /d "%~dp0"

where git >nul 2>nul
if errorlevel 1 (
    echo Git n'est pas installe : installation avec winget...
    winget install -e --id Git.Git --accept-source-agreements --accept-package-agreements
    echo.
    echo Git est installe. FERME cette fenetre et relance 6_push_github.bat
    pause
    exit /b
)

if not exist .git (
    git init -b main
)

set /p URL=Colle le lien de ton depot GitHub (ex: https://github.com/pseudo/depot) puis Entree : 
git remote remove origin >nul 2>nul
git remote add origin %URL%

git config user.name >nul 2>nul || git config user.name "Faouzene"
git config user.email >nul 2>nul || set /p MAIL=Ton email GitHub : 
if defined MAIL git config user.email %MAIL%

git checkout -B campagne
git add -A
git commit -m "FireStationManager : module campagnes de sensibilisation (Qt + Oracle) conforme au MCD"

echo.
echo Une fenetre de connexion GitHub peut s'ouvrir dans le navigateur : connecte-toi.
git push -u origin campagne
if errorlevel 1 (
    echo ***** ERREUR pendant le push : fais une capture et envoie-la a Claude *****
) else (
    echo ===== TERMINE : branche "campagne" envoyee sur GitHub =====
)
pause
