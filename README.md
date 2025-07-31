# RED EYE AntiCheat for HD-Player

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A lightweight, native-Windows anti-cheat monitor for the HD-Player emulator.  
Continuously scans HD-Player processes and attached handles to detect and terminate overlays, unsigned modules, debugger/time-tampering attempts, and other suspicious activity.

## Features

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

## Prerequisites

- Windows 7 or later (x64)  
- Visual Studio 2017 or newer  
- Administrative privileges

## Building

```bash
git clone https://github.com/Galib888/Red-Eye-AntiCheat-for-HD-Player.git
cd Red-Eye-AntiCheat-for-HD-Player
