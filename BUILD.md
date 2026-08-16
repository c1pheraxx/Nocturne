# Build

## Requisitos

- Windows 10/11 x64
- Visual Studio 2019+ com "Desktop development with C++"
- CMake 3.16+

## Método 1: Script automático

Dê 2 cliques em `build.bat` ou execute no CMD:

```cmd
build.bat
```

Detecta o Visual Studio, gera o projeto e compila.

## Método 2: CMake manual

Visual Studio 2022:
```cmd
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Visual Studio 2019:
```cmd
cmake -B build -S . -G "Visual Studio 16 2019" -A x64
cmake --build build --config Release
```

MinGW (sem VS):
```cmd
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
```

## Saída

```
build/Release/
  NocturneDemo.exe
  NocturneTests.exe
```

## Testes

```cmd
build/Release/NocturneTests.exe
```

Ou dê 2 cliques em `test.bat`.
