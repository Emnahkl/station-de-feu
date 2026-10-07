# integrate.ps1 - merges the Campagnes part into the Carte (zones) app
# Run from the repo root:  C:\Users\user\station-de-feu
$ErrorActionPreference = "Continue"

function Say($m) { Write-Host $m -ForegroundColor Cyan }
function Fail($m) { Write-Host "ERROR: $m" -ForegroundColor Red; exit 1 }

if (-not (Test-Path ".git")) { Fail "Run this script from C:\Users\user\station-de-feu" }

$branch = (git branch --show-current).Trim()
if ($branch -ne "integrate/campagne_zone") { Fail "You are on '$branch'. Run: git checkout integrate/campagne_zone" }

if (-not (Select-String -Path "FireStationManager.pro" -Pattern "campagnewindow" -Quiet)) { Fail "FireStationManager.pro was not replaced. Extract the zip into this folder first." }
if (-not (Select-String -Path "src\ui\MainWindow.cpp" -Pattern "CampagneWindow" -Quiet)) { Fail "src\ui\MainWindow.cpp was not replaced. Extract the zip into this folder first." }

function Move-Tracked($from, $to) {
    if ((Test-Path $from) -and -not (Test-Path $to)) {
        git mv $from $to
        if (-not (Test-Path $to)) { Move-Item $from $to }
        Say "renamed $from -> $to"
    }
}

function Edit-File($path, [scriptblock]$fn) {
    $full = (Resolve-Path $path).Path
    $bytes = [System.IO.File]::ReadAllBytes($full)
    $hasBom = ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF)
    $enc = New-Object System.Text.UTF8Encoding($hasBom)
    $text = [System.IO.File]::ReadAllText($full, $enc)
    $new = & $fn $text
    [System.IO.File]::WriteAllText($full, $new, $enc)
}

Say "1/5 Renaming his files..."
Move-Tracked "mainwindow.h"   "campagnewindow.h"
Move-Tracked "mainwindow.cpp" "campagnewindow.cpp"
Move-Tracked "mainwindow.ui"  "campagnewindow.ui"
Move-Tracked "resources.qrc"  "campagne_resources.qrc"
Move-Tracked "main.cpp"       "old_main_campagne.txt"

Say "2/5 Renaming his class MainWindow -> CampagneWindow..."
Edit-File "campagnewindow.h" {
    param($t)
    $t = [regex]::Replace($t, 'MAINWINDOW_H', 'CAMPAGNEWINDOW_H')
    $t = [regex]::Replace($t, 'class\s+MainWindow\s*:\s*public\s+QMainWindow', 'class CampagneWindow : public QMainWindow')
    $t = [regex]::Replace($t, 'explicit\s+MainWindow\s*\(', 'explicit CampagneWindow(')
    $t = [regex]::Replace($t, '~MainWindow\s*\(', '~CampagneWindow(')
    return $t
}
Edit-File "campagnewindow.cpp" {
    param($t)
    $t = [regex]::Replace($t, '#include\s+"mainwindow\.h"', '#include "campagnewindow.h"')
    $t = [regex]::Replace($t, '#include\s+"ui_mainwindow\.h"', '#include "ui_campagnewindow.h"')
    $t = [regex]::Replace($t, '(?<!Ui::)\bMainWindow\b', 'CampagneWindow')
    return $t
}

Say "3/5 Fixing includes in his other root files..."
Get-ChildItem -File -Path . -Include *.cpp,*.h | Where-Object { $_.Name -ne "campagnewindow.cpp" -and $_.Name -ne "campagnewindow.h" } | ForEach-Object {
    $p = $_.FullName
    if (Select-String -Path $p -Pattern 'mainwindow\.h' -Quiet) {
        Edit-File $p {
            param($t)
            $t = [regex]::Replace($t, '"mainwindow\.h"', '"campagnewindow.h"')
            $t = [regex]::Replace($t, '"ui_mainwindow\.h"', '"ui_campagnewindow.h"')
            return $t
        }
        Say ("updated include in " + $_.Name)
    }
}

Say "4/5 Git cleanup..."
if (Test-Path "FireStationZones.pro") { git rm -q -f FireStationZones.pro }
$ignore = ""
if (Test-Path ".gitignore") { $ignore = Get-Content ".gitignore" -Raw }
if ($ignore -notmatch '(?m)^build/') { Add-Content -Path ".gitignore" -Value "`nbuild/" }
if (git ls-files build) { git rm -r -q --cached build }
git add -A
git commit -q -m "Integration: Carte (zones) + Campagnes in one project (FireStationManager.pro)"

Say "5/5 Check: remaining 'MainWindow' in campagnewindow.* (only Ui::MainWindow lines are OK)"
Select-String -Path "campagnewindow.h","campagnewindow.cpp" -Pattern "MainWindow" | ForEach-Object { Write-Host ("  " + $_.Filename + ":" + $_.LineNumber + "  " + $_.Line.Trim()) }

Write-Host ""
Write-Host "DONE. Now open FireStationManager.pro in Qt Creator, then Build > Clean, Run qmake, Rebuild." -ForegroundColor Green
Write-Host ""
Write-Host "===== PASTE EVERYTHING BELOW THIS LINE TO CLAUDE (his old main.cpp) =====" -ForegroundColor Yellow
if (Test-Path "old_main_campagne.txt") { Get-Content "old_main_campagne.txt" }
