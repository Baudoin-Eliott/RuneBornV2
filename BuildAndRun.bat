@echo off
REM Script pour compiler ET lancer RuneBorn V2
cd /d "%~dp0"

echo ========================================
echo Compilation de RuneBorn V2...
echo ========================================
echo.

REM Ajouter MinGW au PATH temporairement
set PATH=C:\msys64\mingw64\bin;%PATH%

REM Se déplacer dans le dossier build
cd build

REM Compiler avec mingw32-make
mingw32-make -j4

REM Revenir à la racine
cd ..

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilation REUSSIE!
    echo Lancement du jeu...
    echo ========================================
    echo.
    start "" "build\RuneBorn.exe"
) else (
    echo.
    echo ========================================
    echo ERREUR de compilation!
    echo Le jeu ne sera pas lance.
    echo ========================================
    pause
)
