@echo off
if "%~1"=="" (
    echo Uso: demo.bat NomeDoJogo.exe
    pause
    exit /b 1
)
build\Release\NocturneDemo.exe %1
