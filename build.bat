@echo off
echo ==========================================
echo Nocturne Build Script
echo ==========================================
echo.

REM Detectar Visual Studio
set VS_PATH=

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
)

if defined VS_PATH (
    echo Configurando ambiente do Visual Studio...
    call %VS_PATH% -arch=amd64
) else (
    echo AVISO: Visual Studio nao encontrado automaticamente.
    echo Tentando compilar com cmake direto...
)

echo.
echo Gerando projeto com CMake...
cmake -B build -S . -G "Visual Studio 17 2022" -A x64

if errorlevel 1 (
    echo.
    echo ERRO: Falha ao gerar projeto.
    echo Tentando com Visual Studio 2019...
    cmake -B build -S . -G "Visual Studio 16 2019" -A x64
)

if errorlevel 1 (
    echo.
    echo ERRO: Nao foi possivel gerar o projeto.
    echo Certifique-se de ter o Visual Studio com C++ instalado.
    pause
    exit /b 1
)

echo.
echo Compilando Release...
cmake --build build --config Release

if errorlevel 1 (
    echo.
    echo ERRO: Falha na compilacao.
    pause
    exit /b 1
)

echo.
echo ==========================================
echo BUILD CONCLUIDO
echo ==========================================
echo Executaveis em: build\Release\
echo.
echo Para rodar os testes:
echo   build\Release\NocturneTests.exe
echo.
echo Para rodar a demo:
echo   build\Release\NocturneDemo.exe NomeDoJogo.exe
echo.
pause
