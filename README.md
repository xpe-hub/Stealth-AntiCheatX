# Stealth AntiCheat X - Advanced AntiCheat for Free Fire Gaming

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build Status](https://github.com/xpe-hub/Stealth-AntiCheatX/workflows/Build%20Stealth-AntiCheatX/badge.svg)](https://github.com/xpe-hub/Stealth-AntiCheatX/actions)

**© 2025 xpe.nettt** - Advanced AntiCheat System for Free Fire Gaming on BlueStacks/MSI

**Stealth-AntiCheatX** es un sistema anti-cheat avanzado y discreto diseñado específicamente para jugadores de Free Fire que utilizan BlueStacks o MSI Emulator. Este programa monitorea y protege contra cheating, aimbots, ESP overlays, DLL injections, y otras técnicas de juego sucio mediante múltiples capas de protección.

## 🚀 **NUEVO**: Compilación Automática

### ✅ **Descarga Inmediata del EXE**

**GitHub Actions compila automáticamente el proyecto:**
- **Push a main** → Compilación automática
- **Create Release** → EXE subido automáticamente  
- **Descarga directa**: https://github.com/xpe-hub/Stealth-AntiCheatX/releases

[🔧 **Guía Completa de Compilación Automática**](COMPILATION_AUTO.md)

## Features

- **External Protection**  
  - Monitors process handles in real time  
  - Terminates windows matching suspicious overlay styles (ESP detection)  
  - Detects and reports unauthorized handle duplication  
- **Internal Protection**  
  - Scans loaded modules for digital signatures (signed vs. unsigned)  
  - Enumerates threads and flags those starting outside known module ranges  
- **Time-Tamper Detection**  
  - Uses steady-clock vs. wall-clock drift to detect speed hacks or debuggers  
- **Self-Protection**  
  - Requires Administrator & SE_DEBUG privileges  
  - Disables console quick-edit and close button  
  - Custom console banner and dynamic title indicating uptime
- **Discord Logging**  
  - Real-time reporting to Discord webhook
  - User tracking and crash attempt monitoring
- **Audio Alerts**  
  - Distinctive beep alerts for each detection type
  - Different frequencies: ESP (1000Hz), DLLs (800Hz), Threads (600Hz), Time (750Hz)
- **Enhanced System Monitoring**  
  - DMA hardware detection (PCI, USB, Serial, Parallel devices)
  - File integrity verification (SHA1 hashing of critical DLLs)
  - Comprehensive system and network information logging

## Prerequisites

- Windows 7 or later (x64)  
- Visual Studio 2017 or newer  
- Administrative privileges

## Building

### Automatic Compilation (Recommended)
**GitHub Actions compiles automatically on every push:**
1. Push to main → Automatic compilation
2. Create Release → EXE automatically uploaded
3. Download from: https://github.com/xpe-hub/Stealth-AntiCheatX/releases

### Manual Compilation
```bash
git clone https://github.com/xpe-hub/Stealth-AntiCheatX.git
cd Stealth-AntiCheatX

# Option 1: MinGW compilation (Windows)
build_mingw.bat

# Option 2: Visual Studio compilation
build_release.bat
```

## Installation

1. Run as Administrator
2. Allow through Windows Defender if prompted
3. Monitor will start automatically

## Usage

Run `Stealth-AntiCheatX.exe` as Administrator. The program will:

- Automatically detect HD-Player processes
- Monitor for ESP overlays and suspicious activity
- Log all activity to console and Discord webhook
- Require no user interaction for basic operation

## Technical Details

- **Target Process**: HD-Player.exe (Free Fire emulator)
- **Detection Method**: Window style enumeration, handle monitoring, module verification
- **Reporting**: Console output + Discord webhook integration
- **Privileges Required**: Administrator + SE_DEBUG

## License

This project is licensed under the MIT License.

---

**Developed by xpe.nettt**  
**Project: Stealth AntiCheat X for Free Fire Gaming**