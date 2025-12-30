@echo off
REM Script pour compiler RuneBorn V2 avec MinGW
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

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilation REUSSIE!
    echo ========================================
    echo.
    echo Executable: build\RuneBorn.exe
    echo Pour lancer le jeu: double-cliquez sur RunGame.bat
    pause
) else (
    echo.
    echo ========================================
    echo ERREUR de compilation!
    echo ========================================
    pause
)

cd ..
