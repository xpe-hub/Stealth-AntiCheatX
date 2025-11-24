@echo off
echo Compilando Stealth-AntiCheatX con MinGW...

REM Crear directorio de salida
mkdir -p Release\x64

REM Compilar con MinGW
g++ -o Release\x64\Stealth-AntiCheatX.exe Stealth-AntiCheatX.cpp \
    -lws2_32 -lwinhttp -lwinmm -lversion -lwininet \
    -static -static-libgcc -static-libstdc++ \
    -O2 -DNDEBUG \
    -std=c++17 \
    -Wall -Wextra

if %ERRORLEVEL% EQU 0 (
    echo [ÉXITO] EXE compilado en Release\x64\Stealth-AntiCheatX.exe
) else (
    echo [ERROR] Error en la compilación
    exit /b %ERRORLEVEL%
)