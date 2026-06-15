@echo off
REM ============================================================
REM build.bat  -  Compilacion de BitQuest en Windows
REM Usa el GCC de MSYS2 (64 bits)
REM ============================================================

SET GCC=C:\msys64\ucrt64\bin\gcc.exe
SET NASM=nasm

echo [1/3] Ensamblando rutinas.asm...
%NASM% -f win64 rutinas.asm -o rutinas.obj
IF ERRORLEVEL 1 (
    echo ERROR: fallo al ensamblar rutinas.asm
    pause
    exit /b 1
)

echo [2/3] Compilando y enlazando con GCC 64-bit...
%GCC% -Wall -o BitQuest.exe main.c juego.c rutinas.obj
IF ERRORLEVEL 1 (
    echo ERROR: fallo al compilar con GCC
    pause
    exit /b 1
)

echo [3/3] Listo! Ejecuta BitQuest.exe para jugar.
pause