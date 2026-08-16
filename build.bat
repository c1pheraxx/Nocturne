@echo off
chcp 65001 >nul
echo ==========================================
echo Nocturne Build Script (Atualizado)
echo ==========================================
echo.

REM ==========================================
REM DETECCAO DO VISUAL STUDIO
REM ==========================================
set VS_PATH=
set VS_VERSION=

REM --- Visual Studio 2026 (v18) ---
if exist "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
    set VS_VERSION=2026 Community
) else if exist "C:\Program Files\Microsoft Visual Studio\18\Professional\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\18\Professional\Common7\Tools\VsDevCmd.bat"
    set VS_VERSION=2026 Professional
) else if exist "C:\Program Files\Microsoft Visual Studio\18\Enterprise\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\18\Enterprise\Common7\Tools\VsDevCmd.bat"
    set VS_VERSION=2026 Enterprise
) else if exist "C:\Program Files\Microsoft Visual Studio\18\Preview\Common7\Tools\VsDevCmd.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\18\Preview\Common7\Tools\VsDevCmd.bat"
    set VS_VERSION=2026 Preview
)

REM --- Visual Studio 2022 (v17) ---
if not defined VS_PATH (
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2022 Community
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2022 Professional
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2022 Enterprise
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Preview\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Preview\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2022 Preview
    )
)

REM --- Visual Studio 2019 (v16) ---
if not defined VS_PATH (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2019 Community
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2019 Professional
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2019 Enterprise
    )
)

REM --- Build Tools ---
if not defined VS_PATH (
    if exist "C:\Program Files\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2026 BuildTools
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
        set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
        set VS_VERSION=2022 BuildTools
    )
)

if defined VS_PATH (
    echo [OK] Visual Studio %VS_VERSION% encontrado.
    echo [OK] Configurando ambiente...
    call %VS_PATH% -arch=amd64
    if errorlevel 1 (
        echo [ERRO] Falha ao configurar ambiente.
        pause
        exit /b 1
    )
) else (
    echo [AVISO] VS nao encontrado automaticamente.
)

echo.

REM ==========================================
REM DETECCAO DO CMAKE
REM ==========================================
where cmake >nul 2>nul
if errorlevel 1 (
    echo [ERRO] CMake nao encontrado.
    pause
    exit /b 1
)
for /f "tokens=3" %%a in ('cmake --version ^| findstr /i "version"') do set CMAKE_VER=%%a
echo [OK] CMake %CMAKE_VER%

REM ==========================================
REM LIMPEZA DO CACHE
REM ==========================================
echo.
echo [INFO] Limpando cache antigo...
if exist "build\CMakeCache.txt" del /f /q "build\CMakeCache.txt" >nul 2>nul
if exist "build\CMakeFiles" rmdir /s /q "build\CMakeFiles" >nul 2>nul
if exist "out" rmdir /s /q "out" >nul 2>nul

REM ==========================================
REM GERACAO DO PROJETO
REM ==========================================
echo.
echo [INFO] Gerando projeto...

if defined VS_VERSION (
    echo %VS_VERSION% | findstr /i "2026" >nul
    if not errorlevel 1 (
        cmake -B build -S . -G "Visual Studio 18 2026" -A x64
        if not errorlevel 1 goto BUILD
        cmake -B build -S . -G "Visual Studio 17 2022" -A x64
        if not errorlevel 1 goto BUILD
        goto CMAKE_FAIL
    )
)

if defined VS_VERSION (
    echo %VS_VERSION% | findstr /i "2022" >nul
    if not errorlevel 1 (
        cmake -B build -S . -G "Visual Studio 17 2022" -A x64
        if not errorlevel 1 goto BUILD
        goto CMAKE_FAIL
    )
)

cmake -B build -S . -G "Visual Studio 17 2022" -A x64
if not errorlevel 1 goto BUILD

:CMAKE_FAIL
echo.
echo [ERRO] Falha ao gerar projeto.
pause
exit /b 1

:BUILD
echo.
echo [OK] Projeto gerado!

REM ==========================================
REM COMPILACAO
REM ==========================================
echo [INFO] Compilando Release...
cmake --build build --config Release

if errorlevel 1 (
    echo.
    echo [ERRO] Falha na compilacao.
    pause
    exit /b 1
)

echo.
echo ==========================================
echo BUILD CONCLUIDO COM SUCESSO!
echo ==========================================
echo.
echo Executaveis em: build\Release\
echo.
pause
