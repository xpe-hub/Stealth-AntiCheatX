#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>

// Anti-Cracking Protection System - Simplified for MinGW-w64
class AntiCrackProtection {
private:
    bool cracking_detected = false;
    std::vector<std::string> dangerous_processes = {
        "ollydbg.exe", "x64dbg.exe", "windbg.exe", "cheatengine.exe",
        "processhacker.exe", "hxd.exe", "hexedit.exe"
    };

public:
    AntiCrackProtection() {
        InitializeProtection();
    }

    void InitializeProtection() {
        // Verificación básica de debuggers
        DetectBasicDebuggers();
        
        // Monitoring will be started manually by caller
    }

    void DetectBasicDebuggers() {
        // IsDebuggerPresent
        if (IsDebuggerPresent()) {
            TriggerCountermeasures("Debugger detected");
        }

        // Timing check
        auto start = std::chrono::high_resolution_clock::now();
        volatile int dummy = 0;
        for (int i = 0; i < 10000; i++) dummy++;
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        if (duration.count() > 1000000) { // 1ms threshold
            TriggerCountermeasures("Timing anomaly detected");
        }
    }

    void CheckForCrackingTools() {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 processEntry = { 0 };
            processEntry.dwSize = sizeof(PROCESSENTRY32);
            
            if (Process32First(snapshot, &processEntry)) {
                do {
                    // Convertir WCHAR a string simple
                    std::string processName;
                    for (int i = 0; i < 260 && processEntry.szExeFile[i] != L'\0'; i++) {
                        if (processEntry.szExeFile[i] < 256) {
                            processName += (char)processEntry.szExeFile[i];
                        }
                    }
                    
                    for (const auto& dangerous : dangerous_processes) {
                        if (processName.find(dangerous) != std::string::npos) {
                            TriggerCountermeasures("Cracking tool detected: " + processName);
                            break;
                        }
                    }
                } while (Process32Next(snapshot, &processEntry));
            }
        }
        CloseHandle(snapshot);
    }

    void TriggerCountermeasures(const std::string& reason) {
        if (cracking_detected) return;
        cracking_detected = true;
        
        // Log del intento
        std::string log = "[SECURITY] Crack attempt detected: " + reason + 
                         " | Time: " + std::to_string(GetTickCount());
        
        PerformCountermeasures(reason);
    }

    void PerformCountermeasures(const std::string& reason) {
        // Countermeasure 1: Performance degradation (CPU stress)
        for (int i = 0; i < 2; i++) {
            // Simulate CPU stress without threading
            volatile long long result = 0;
            for (int j = 0; j < 100000; j++) {
                result += j;
            }
        }

        // Countermeasure 2: Memory pressure (moderado)
        std::vector<char*> memory_blocks;
        for (int i = 0; i < 20; i++) {
            char* block = new char[5 * 1024 * 1024]; // 5MB blocks
            memset(block, 0xFF, 5 * 1024 * 1024);
            memory_blocks.push_back(block);
        }

        // Countermeasure 3: Registry access storm (light)
        for (int i = 0; i < 100; i++) {
            HKEY hKey;
            RegOpenKeyExA(HKEY_CURRENT_USER, "Software", 0, KEY_ALL_ACCESS, &hKey);
            if (hKey) RegCloseKey(hKey);
        }

        // Countermeasure 4: File system activity (controlado)
        char tempPath[MAX_PATH];
        GetTempPathA(MAX_PATH, tempPath);
        for (int i = 0; i < 20; i++) {
            std::string tempFile = std::string(tempPath) + "anticheat_temp_" + std::to_string(i) + ".tmp";
            HANDLE hFile = CreateFileA(tempFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                WriteFile(hFile, "CRACK_ATTEMPT_DETECTED", 25, NULL, NULL);
                CloseHandle(hFile);
            }
        }

        // Countermeasure 5: Beep alerts
        for (int i = 0; i < 3; i++) {
            Beep(200, 100);
            Beep(300, 100);
            Beep(200, 100);
        }

        // Mostrar mensaje de warning
        MessageBoxA(NULL, 
            "Unauthorized modification detected!\n\nThis software is protected.\nPlease use the licensed version.", 
            "Stealth AntiCheat X - Security Warning", 
            MB_OK | MB_ICONWARNING);
    }

    // Manual monitoring called by main program
    void CheckForThreats() {
        if (!cracking_detected) {
            CheckForCrackingTools();
            DetectBasicDebuggers();
        }
    }

    bool IsCrackingDetected() const {
        return cracking_detected;
    }

    ~AntiCrackProtection() {
        // Cleanup
    }
};

// Función para validar licencia online (placeholder)
bool ValidateOnlineLicense(const std::string& license_key, const std::string& hardware_fingerprint) {
    // En implementación real, contactar servidor de licencias
    return !license_key.empty() && hardware_fingerprint.length() > 10;
}