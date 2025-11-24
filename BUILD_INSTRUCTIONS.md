# Stealth-AntiCheatX - BUILD INSTRUCTIONS

## 🎯 Quick Build (Recommended)

```bash
# Double-click this file to build
build_release.bat
```

## 📋 Manual Build Steps

1. **Open Command Prompt as Administrator**
   ```cmd
   cd "C:\path\to\Stealth-AntiCheatX"
   ```

2. **Build Release x64**
   ```cmd
   msbuild.exe Stealth-AntiCheatX.sln /p:Configuration=Release /p:Platform=x64
   ```

3. **Run the executable**
   ```cmd
   cd Release\x64
   Stealth-AntiCheatX.exe
   ```

## 🔧 Prerequisites

- **Visual Studio 2017 or newer**
- **Windows SDK 10.0**
- **Administrative privileges**

## ✅ Verified Features

### 🔊 Audio Alerts
- ✅ **ESP Overlay Detection**: 1000Hz, 500ms beep
- ✅ **Unsigned Executables**: 800Hz, 400ms beep  
- ✅ **Unsigned DLLs**: 800Hz, 400ms beep
- ✅ **Suspicious Threads**: 600Hz, 300ms beep
- ✅ **Time Tampering**: 750Hz, 300ms beep

### 🛡️ Security Features  
- ✅ **DMA Hardware Detection**: Moderate, informational scanning
- ✅ **File Integrity Verification**: SHA1 of critical system files
- ✅ **Enhanced Discord Logging**: System details and network info
- ✅ **DLL Injection Detection**: Scans ALL modules in HD-Player.exe
- ✅ **Thread Verification**: Detects threads outside module ranges

### 🎯 Target Process: HD-Player.exe
- ✅ **Process Handle Monitoring**: Real-time handle analysis
- ✅ **Window Style Detection**: ESP overlay termination (0x94000000)
- ✅ **Module Enumeration**: Checks ALL loaded DLLs
- ✅ **Signature Verification**: Digital signature validation
- ✅ **Network Integration**: Discord webhook logging

## 🚀 Deployment

1. **Compile**: Use `build_release.bat` or manual build
2. **Run as Administrator**: Essential for process access
3. **Allow in Windows Defender**: May trigger false positives
4. **Monitor Discord**: Real-time logging to webhook

## 📁 Output Structure

```
Release/
└── x64/
    └── Stealth-AntiCheatX.exe (Ready to deploy)
```

## ⚠️ Important Notes

- **Run as Administrator**: Required for process monitoring
- **Windows Defender**: May flag as suspicious (false positive)
- **Discord Webhook**: Already configured and active
- **Audio Alerts**: All detection types have beep alerts

---

**Developed By xpe.nettt**  
**© 2025 - Stealth AntiCheat X for Free Fire Gaming**