@echo off
echo ===========================================
echo Stealth-AntiCheatX - Release Build Script
echo Developed By xpe.nettt
echo ===========================================
echo.

if not exist Release\x64 mkdir Release\x64

echo Compiling Release x64 configuration...
msbuild.exe Stealth-AntiCheatX.sln /p:Configuration=Release /p:Platform=x64 /m

if %errorlevel% equ 0 (
    echo.
    echo ✅ BUILD SUCCESSFUL!
    echo ✅ Executable created: Release\x64\Stealth-AntiCheatX.exe
    echo.
    echo 🔊 AUDIO ALERTS CONFIGURED:
    echo    - ESP Detection: 1000Hz, 500ms
    echo    - DLL Unsigned: 800Hz, 400ms
    echo    - Suspicious Threads: 600Hz, 300ms
    echo    - Time Tampering: 750Hz, 300ms
    echo.
    echo 🛡️ SECURITY FEATURES:
    echo    - DMA Hardware Detection
    echo    - File Integrity Verification
    echo    - Enhanced Discord Logging
    echo    - Comprehensive DLL Scanning
    echo.
    echo ⚠️ IMPORTANT: Run as Administrator!
    echo.
) else (
    echo.
    echo ❌ BUILD FAILED!
    echo Please check the error messages above.
    echo.
)

pause