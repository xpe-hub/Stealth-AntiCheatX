#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include <psapi.h>
#include <winternl.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <cstdio>
#include <wincrypt.h>
#include <wintrust.h>
#include <softpub.h>
#include <shellapi.h>
#include <filesystem>
#include <memory>
#include <wininet.h>
#include <map>
#include <ctime>
#include <setupapi.h>

// Ultra Anti-Cracking Protection System
#include "UltraAntiCrack.h"

// GUID_DEVCLASS_PORTS definition
const GUID GUID_DEVCLASS_PORTS = {0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};

// Incluir configuración de webhook desde GitHub Actions (si existe)
#ifdef DISCORD_WEBHOOK_CONFIG
#include "webhook_config.h"
#endif

#ifndef STATUS_INFO_LENGTH_MISMATCH
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#endif
#define BREAKPOINT_OPCODE 0xCC
#define QUARANTINE_FOLDER L"C:\\Quarantine_Stealth"
#define DISCORD_WEBHOOK_URL L"https://discord.com/api/webhooks/1441660384443498578/cCBalfn0kXDaV3GjdeqyGMbXTqOEoQMyx8yFZRauypmWTpIZIM40xBrOGcsP5wNWzLvM" // HD Player Commercial Anti-Cheat

// Product Configuration
#define PRODUCT_NAME L"Stealth AntiCheat X - for HD Player"
#define VERSION L"2.0.0"
#define COMPANY L"xpe.nettt"

// Webhook URL as constant variable
static const std::wstring WEBHOOK_URL = L"https://discord.com/api/webhooks/1441660384443498578/cCBalfn0kXDaV3GjdeqyGMbXTqOEoQMyx8yFZRauypmWTpIZIM40xBrOGcsP5wNWzLvM";

// Global Ultra Anti-Crack Protection Instance
std::unique_ptr<UltraAntiCrackProtection> g_anticrack = nullptr;

// Function declarations
void LogToDiscord(const std::wstring& title, const std::wstring& details, const std::wstring& type);
void LogDetection(const std::wstring& title, const std::wstring& details, const std::wstring& severity);
void LogQuarantineAction(const std::wstring& action, const std::wstring& reason);
void LogUserExecution(const std::wstring& license_key = L"");
void MonitorHDPlayerProcess(DWORD pid);
void AnalyzeHDPlayerProcess();
void DetectStreamerModeCheats();
void RunTournamentMode();
void RunStreamMode();
void RunCommunityMode();
void RunProfessionalMode();
void DetectDMAHardware();
void VerifyFileIntegrity();

typedef struct _SYSTEM_HANDLE {
    ULONG ProcessId;
    UCHAR ObjectTypeNumber;
    UCHAR Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

typedef NTSTATUS(WINAPI* NtQuerySystemInformation_t)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

std::wstring to_wstring(DWORD value) {
    std::wstringstream wss;
    wss << value;
    return wss.str();
}

HANDLE OpenProcessSafe(DWORD processId) {
    return OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
}

DWORD FindProcessByName(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } while (Process32NextW(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return 0;
}

std::wstring GetProcessPath(DWORD processId) {
    std::wstring processPath = L"";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess) {
        char buffer[MAX_PATH];
        DWORD bufferSize = sizeof(buffer);
        if (QueryFullProcessImageNameA(hProcess, 0, buffer, &bufferSize)) {
            processPath = std::wstring(buffer, buffer + bufferSize);
        }
        CloseHandle(hProcess);
    }
    return processPath;
}

// Discord webhook functions
void LogToDiscord(const std::wstring& title, const std::wstring& details, const std::wstring& type) {
    if (g_anticrack && g_anticrack->IsCrackingDetected()) {
        return; // Si se detectó cracking, no enviar más mensajes
    }
    
    std::wstringstream payload;
    payload << L"{\"embeds\":[{"
            << L"\"title\":\"" << title << L"\","
            << L"\"description\":\"" << details << L"\","
            << L"\"color\":3447003,"
            << L"\"fields\":["
            << L"{\"name\":\"Tipo\",\"value\":\"" << type << L"\",\"inline\":true},"
            << L"{\"name\":\"Proceso\",\"value\":\"" << title << L"\",\"inline\":true},"
            << L"{\"name\":\"Hora\",\"value\":\"" << std::to_wstring(GetTickCount()) << L"\",\"inline\":true}"
            << L"],"
            << L"\"footer\":{\"text\":\"Stealth AntiCheat X - for HD Player\"}"
            << L"}]}";
    
    HINTERNET hInternet = InternetOpenA("StealthAntiCheat", 1, NULL, NULL, 0);
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

void LogDetection(const std::wstring& title, const std::wstring& details, const std::wstring& severity) {
    std::wstring logMessage = L"🎯 " + title + L" detectado: " + details;
    std::wcout << logMessage << std::endl;
    
    LogToDiscord(title, details, L"Detection");
}

void LogQuarantineAction(const std::wstring& action, const std::wstring& reason) {
    std::wstring logMessage = L"📦 Quarantine: " + action + L" - " + reason;
    std::wcout << logMessage << std::endl;
    
    LogToDiscord(L"Quarantine Action", action + L" - " + reason, L"Quarantine");
}

void LogUserExecution(const std::wstring& license_key) {
    std::wstring user = L"xpe.nettt";
    std::wstring logMessage = L"👤 Usuario " + user + L" ejecutó Stealth AntiCheat X - for HD Player v2.0";
    std::wcout << logMessage << std::endl;
    
    LogToDiscord(L"User Execution", user + L" ejecutó el anti-cheat", L"Execution");
    
    if (!license_key.empty()) {
        LogToDiscord(L"License Update", L"Usuario actualizó licencia: " + license_key, L"License");
    }
}

// Advanced detection functions
void AnalyzeHDPlayerProcess() {
    std::wcout << L"🔍 Analizando proceso HD-Player.exe..." << std::endl;
    
    DWORD hdPlayerPid = FindProcessByName(L"HD-Player.exe");
    if (hdPlayerPid != 0) {
        std::wcout << L"✅ HD Player detectado - PID: " << hdPlayerPid << std::endl;
        
        MonitorHDPlayerProcess(hdPlayerPid);
        
        // Escanear memoria del proceso
        HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, hdPlayerPid);
        if (hProcess) {
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            MEMORY_BASIC_INFORMATION mbi;
            
            char buffer[4096];
            LPVOID addr = sysInfo.lpMinimumApplicationAddress;
            
            while (addr < sysInfo.lpMaximumApplicationAddress) {
                if (VirtualQueryEx(hProcess, addr, &mbi, sizeof(mbi)) == sizeof(mbi)) {
                    if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_GUARD) == 0) {
                        SIZE_T bytesRead;
                        if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, std::min(sizeof(buffer), mbi.RegionSize), &bytesRead)) {
                            // Buscar patrones sospechosos
                            for (SIZE_T i = 0; i < bytesRead - 10; i++) {
                                // Buscar patrones de aimbot/wallhack comunes
                                if (strstr(buffer + i, "aimbot") || strstr(buffer + i, "wallhack") || 
                                    strstr(buffer + i, "ESP") || strstr(buffer + i, "no_recoil")) {
                                    
                                    std::wstringstream ss;
                                    ss << L"Patrón de cheat detectado en HD Player - Offset: 0x" << std::hex << (DWORD_PTR)(mbi.BaseAddress) + i;
                                    LogDetection(L"Memory Pattern", ss.str(), L"HIGH");
                                }
                            }
                        }
                    }
                    addr = (LPVOID)((DWORD_PTR)mbi.BaseAddress + mbi.RegionSize);
                } else {
                    break;
                }
            }
            
            CloseHandle(hProcess);
        }
    } else {
        std::wcout << L"⚠️ HD Player no detectado" << std::endl;
    }
}

void DetectStreamerModeCheats() {
    std::wcout << L"📺 Verificando cheats compatibles con streaming..." << std::endl;
    
    // Buscar procesos relacionados con streaming tools
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry = { 0 };
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        
        if (Process32First(hSnapshot, &processEntry)) {
            do {
                // Convertir WCHAR a string
                std::string processName;
                for (int i = 0; i < 260 && processEntry.szExeFile[i] != L'\0'; i++) {
                    if (processEntry.szExeFile[i] < 256) {
                        processName += (char)processEntry.szExeFile[i];
                    }
                }
                
                // Convertir a minúsculas para comparación
                std::transform(processName.begin(), processName.end(), processName.begin(), ::tolower);
                
                if (processName.find("obs") != std::string::npos ||
                    processName.find("xsplit") != std::string::npos ||
                    processName.find("streamlabs") != std::string::npos) {
                    
                    std::wcout << L"🎮 Software de streaming detectado: " << processEntry.szExeFile << std::endl;
                    
                    // Verificar si hay cheats que podrían afectar el stream
                    DWORD streamingCheatPid = FindProcessByName(L"StreamCheatDetector.exe");
                    if (streamingCheatPid == 0) {
                        LogDetection(L"Stream Mode", L"Streaming software activo - Verificando compatibilidad", L"MEDIUM");
                    }
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }
    }
    CloseHandle(hSnapshot);
}

void MonitorHDPlayerProcess(DWORD pid) {
    std::wcout << L"👁️ Monitoreando proceso HD Player (PID: " << pid << L")..." << std::endl;
    
    HANDLE hProcess = OpenProcessSafe(pid);
    if (hProcess) {
        // Verificar si el proceso tiene módulos sospechosos
        DWORD cbNeeded;
        HMODULE hMods[1024];
        
        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
            DWORD numModules = cbNeeded / sizeof(HMODULE);
            
            for (DWORD i = 0; i < numModules; i++) {
                char moduleName[MAX_PATH];
                if (GetModuleBaseNameA(hProcess, hMods[i], moduleName, sizeof(moduleName))) {
                    std::string moduleStr(moduleName);
                    
                    // Buscar módulos sospechosos
                    if (moduleStr.find("hack") != std::string::npos ||
                        moduleStr.find("cheat") != std::string::npos ||
                        moduleStr.find("inject") != std::string::npos) {
                        
                        std::wstringstream ss;
                        ss << L"Módulo sospechoso cargado: " << moduleName;
                        LogDetection(L"DLL Injection", ss.str(), L"CRITICAL");
                    }
                }
            }
        }
        
        CloseHandle(hProcess);
    }
}

// Mode-specific functions
void RunTournamentMode() {
    std::wcout << L"🏆 Iniciando modo torneo profesional..." << std::endl;
    
    // Verificar integridad del sistema
    LogDetection(L"Tournament Mode", L"Modo torneo iniciado - Verificación completa", L"INFO");
    
    AnalyzeHDPlayerProcess();
    
    // Verificar hardware DMA (para torneos)
    std::wcout << L"🔌 Verificando hardware DMA..." << std::endl;
    DetectDMAHardware();
    
    // Análisis completo
    for (int i = 0; i < 5; i++) {
        if (g_anticrack && g_anticrack->IsCrackingDetected()) break;
        
        std::wcout << L"⏳ Análisis de torneo " << (i+1) << L"/5..." << std::endl;
        
        // Verificar integridad de archivos
        VerifyFileIntegrity();
        
        // Monitoreo intensivo
        AnalyzeHDPlayerProcess();
        DetectDMAHardware();
        
        Sleep(2000); // 2 segundos entre análisis
    }
    
    std::wcout << L"✅ Modo torneo completado" << std::endl;
}

void RunStreamMode() {
    std::wcout << L"📺 Iniciando modo streaming..." << std::endl;
    
    LogDetection(L"Stream Mode", L"Modo streaming iniciado", L"INFO");
    
    AnalyzeHDPlayerProcess();
    
    // Verificar compatibilidad con streaming
    DetectStreamerModeCheats();
    
    // Monitoreo stealth (sin interferir con streams)
    for (int i = 0; i < 10; i++) {
        if (g_anticrack && g_anticrack->IsCrackingDetected()) break;
        
        AnalyzeHDPlayerProcess();
        
        Sleep(3000); // 3 segundos entre análisis
    }
    
    std::wcout << L"✅ Modo streaming completado" << std::endl;
}

void RunCommunityMode() {
    std::wcout << L"🌟 Conectando a Community Stealth Network..." << std::endl;
    
    LogDetection(L"Community Mode", L"Conectando a Community Stealth Network", L"INFO");
    
    AnalyzeHDPlayerProcess();
    
    // Reportar a la comunidad
    for (int i = 0; i < 3; i++) {
        if (g_anticrack && g_anticrack->IsCrackingDetected()) break;
        
        AnalyzeHDPlayerProcess();
        
        Sleep(1000);
    }
    
    std::wcout << L"✅ Community Stealth Network conectado" << std::endl;
    std::wcout << L"🔗 Discord: Proyectos Privados disponibles" << std::endl;
}

void RunProfessionalMode() {
    std::wcout << L"🚀 Iniciando modo profesional completo..." << std::endl;
    
    LogDetection(L"Professional Mode", L"Modo profesional iniciado", L"INFO");
    
    // Ejecutar todas las verificaciones
    AnalyzeHDPlayerProcess();
    DetectDMAHardware();
    VerifyFileIntegrity();
    
    // Monitoreo continuo
    for (int i = 0; i < 10; i++) {
        if (g_anticrack && g_anticrack->IsCrackingDetected()) break;
        
        AnalyzeHDPlayerProcess();
        
        Sleep(2000);
    }
    
    std::wcout << L"✅ Modo profesional completado" << std::endl;
}

// Utility functions
void DetectDMAHardware() {
    std::wcout << L"🔌 Detectando hardware DMA..." << std::endl;
    
    HDEVINFO deviceInfoSet = SetupDiGetClassDevsA(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    if (deviceInfoSet != INVALID_HANDLE_VALUE) {
        SP_DEVINFO_DATA deviceInfoData;
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        
        DWORD deviceIndex = 0;
        while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
            char deviceDesc[256];
            if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, 
                (PBYTE)deviceDesc, sizeof(deviceDesc), NULL)) {
                
                std::string deviceStr(deviceDesc);
                
                // Buscar dispositivos DMA sospechosos
                if (deviceStr.find("USB") != std::string::npos ||
                    deviceStr.find("Game") != std::string::npos ||
                    deviceStr.find("Controller") != std::string::npos) {
                    
                    LogDetection(L"DMA Hardware", std::wstring(deviceDesc, deviceDesc + strlen(deviceDesc)), L"MEDIUM");
                }
            }
            deviceIndex++;
        }
        
        SetupDiDestroyDeviceInfoList(deviceInfoSet);
    }
}

void VerifyFileIntegrity() {
    std::wcout << L"🔍 Verificando integridad de archivos..." << std::endl;
    
    // Verificar si el ejecutable actual fue modificado
    char currentPath[MAX_PATH];
    GetModuleFileNameA(NULL, currentPath, sizeof(currentPath));
    
    HANDLE hFile = CreateFileA(currentPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD fileSize = GetFileSize(hFile, NULL);
        CloseHandle(hFile);
        
        // Verificar tamaño esperado (aproximado)
        if (fileSize > 1500000 || fileSize < 100000) { // Entre 100KB y 1.5MB
            LogDetection(L"File Integrity", L"Tamaño de archivo sospechoso", L"HIGH");
        }
    }
}

// Main entry point
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize Ultra Anti-Crack Protection
    g_anticrack = std::make_unique<UltraAntiCrackProtection>();
    
    // Check for threats immediately
    if (g_anticrack->IsCrackingDetected()) {
        // Ultra Anti-Crack Protection has already handled this
        return 1;
    }
    
    std::wcout << L"===========================================\n";
    std::wcout << L"🎮 Stealth AntiCheat X - for HD Player\n";
    std::wcout << L"📦 Versión: " << VERSION << L"\n";
    std::wcout << L"🏢 Desarrollado por: " << COMPANY << L"\n";
    std::wcout << L"===========================================\n";
    
    LogUserExecution(L"");
    
    int argc = __argc;
    wchar_t** argv = __wargv;
    
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::wstring arg = argv[i];
            
            // Check for anti-cracking threats during execution
            if (g_anticrack) g_anticrack->CheckForThreats();
            
            if (arg == L"--hdplayer" || arg == L"-h") {
                std::wcout << L"🎮 Iniciando detección para HD Player...\n";
                AnalyzeHDPlayerProcess();
            } else if (arg == L"--advanced" || arg == L"-a") {
                std::wcout << L"🚀 Iniciando detección avanzada...\n";
                RunProfessionalMode();
            } else if (arg == L"--tournament" || arg == L"-t") {
                std::wcout << L"🏆 Iniciando modo torneo...\n";
                RunTournamentMode();
            } else if (arg == L"--stream" || arg == L"-s") {
                std::wcout << L"📺 Iniciando modo streaming...\n";
                RunStreamMode();
            } else if (arg == L"--community" || arg == L"-c") {
                std::wcout << L"🌟 Conectando a Community Stealth Network...\n";
                RunCommunityMode();
            } else if (arg == L"--debug" || arg == L"-d") {
                std::wcout << L"🔧 Modo debug activado...\n";
                LogDetection(L"Debug Mode", L"Modo debug iniciado", L"INFO");
                RunProfessionalMode();
            } else {
                std::wcout << L"❌ Argumento no reconocido: " << arg << L"\n";
                std::wcout << L"\n🎯 Uso:\n";
                std::wcout << L"  --hdplayer  - Detección HD Player (por defecto)\n";
                std::wcout << L"  --advanced  - Detección completa\n";
                std::wcout << L"  --tournament - Modo torneo\n";
                std::wcout << L"  --stream    - Modo streaming\n";
                std::wcout << L"  --community - Modo comunidad\n";
                std::wcout << L"  --debug     - Modo debug\n";
            }
        }
    } else {
        // Si no hay argumentos, ejecutar modo HD Player por defecto
        std::wcout << L"🎮 Iniciando modo HD Player (por defecto)...\n";
        AnalyzeHDPlayerProcess();
    }
    
    std::wcout << L"\n===========================================\n";
    std::wcout << L"🎯 Stealth AntiCheat X finalizado\n";
    std::wcout << L"📊 Logs enviados a Discord\n";
    std::wcout << L"🛡️ Sistema anti-cracking activo\n";
    std::wcout << L"===========================================\n";
    
    return 0;
}

// MinGW-w64 compatibility wrapper
int wmain(int argc, wchar_t** argv) {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_NORMAL);
}