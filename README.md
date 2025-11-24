# Stealth AntiCheat X - Advanced AntiCheat for BlueStacks

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build Status](https://github.com/xpe-hub/Stealth-AntiCheatX/workflows/Build%20Stealth-AntiCheatX/badge.svg)](https://github.com/xpe-hub/Stealth-AntiCheatX/actions)

**© 2025 xpe.nettt** - Professional Anti-Cheat System for BlueStacks/MSI

**Stealth-AntiCheatX** es un sistema de protección anti-cheat avanzado diseñado para organizadores de torneos y competiciones competitivas. Previene wallhacks, aimbot, ESP, DLL injection, y otras técnicas de trampa mediante múltiples capas de protección en tiempo real, evitando xiteos y clk coups en competiciones profesionales.

## 🚀 **NUEVO**: Compilación Automática

### ✅ **Descarga Inmediata del EXE**

**GitHub Actions compila automáticamente el proyecto:**
- **Push a main** → Compilación automática
- **Create Release** → EXE subido automáticamente  
- **Descarga directa**: https://github.com/xpe-hub/Stealth-AntiCheatX/releases

[🔧 **Guía Completa de Compilación Automática**](COMPILATION_AUTO.md)

## 🎯 **Categorías de Detección Profesional**

### **Wallhacks & ESP Detection**
- ✅ Detecta overlays transparentes estilo ESP
- ✅ Monitorea ventanas con estilos sospechosos (0x94000000)
- ✅ Terminación automática de procesos ESP activos

### **Aimbot & Process Injection**
- ✅ Escaneo de DLLs no firmadas en tiempo real
- ✅ Detección de inyección de procesos externa
- ✅ Verificación de threads sospechosos fuera de rangos conocidos

### **Hardware Cheats & DMA**
- ✅ Detección de dispositivos DMA (PCI, USB, Serial, Parallel)
- ✅ Monitoreo de hardware de terceros no autorizado
- ✅ Análisis de dispositivos periféricos sospechosos

### **Speed Hacks & Time Manipulation**
- ✅ Detección de tampering temporal (steady-clock vs wall-clock)
- ✅ Identificación de debuggers y velocidad manipulation
- ✅ Verificación de integridad temporal del sistema

### **File Integrity & System Monitoring**
- ✅ Verificación SHA1 de DLLs críticas del sistema
- ✅ Monitoreo de modificaciones no autorizadas
- ✅ Logging completo de actividad del sistema

## 🛡️ **Funcionalidades de Protección**

- **External Protection**  
  - Monitors process handles in real time  
  - Terminates windows matching suspicious overlay styles  
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
- **Discord Community Integration**  
  - **Stealth Gaming Community**: https://discord.gg/3sCxhWShvu
  - Real-time reporting to Discord webhook
  - User tracking and crash attempt monitoring
- **Professional Audio Alerts**  
  - Distinctive beep alerts for each detection category
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

## 📊 **Información Técnica**

- **Target Process**: HD-Player.exe (BlueStacks emulator)
- **Detection Method**: Window style enumeration, handle monitoring, module verification
- **Reporting**: Console output + Discord webhook integration
- **Privileges Required**: Administrator + SE_DEBUG
- **Community**: [Stealth Gaming Community](https://discord.gg/3sCxhWShvu)

## License

This project is licensed under the MIT License.

---

**Developed by xpe.nettt**  
**Project: Stealth AntiCheat X for BlueStacks**