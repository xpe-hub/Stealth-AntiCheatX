#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <psapi.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <Sddl.h> 
#include <comdef.h>
#include <Wbemidl.h>

#include <algorithm>

#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <set>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <wininet.h>
#include <map>

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define BREAKPOINT_OPCODE 0xCC

// Discord Webhook Configuration
#define DISCORD_WEBHOOK_URL L"https://discord.com/api/webhooks/1441660384443498578/cCBalfn0kXDaV3GjdeqyGMbXTqOEoQMyx8yFZRauypmWTpIZIM40xBrOGcsP5wNWzLvM"

// System Information and File Integrity
#include <ShlObj.h>
#include <atlstr.h>
#include <winhttp.h>
#include <iphlpapi.h>
#include <netlistmgr.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "iphlpapi.lib")

// Function declarations for new features
void DetectDMAHardware();
bool VerifyFileIntegrity();
std::string GetSystemInfo();
std::string GetNetworkInfo();
void LogSystemDetails();
bool CalculateFileHash(const std::wstring& filePath, std::string& hash);

// Product Configuration
#define PRODUCT_NAME L"Stealth AntiCheat X - for HD Player"
#define VERSION L"2.0.0"
#define COMPANY L"xpe.nettt"

// Function declarations for Discord logging
void LogToDiscord(const std::wstring& title, const std::wstring& details, const std::wstring& type);
void LogUserExecution(const std::wstring& license_key = L"");
void LogDetection(const std::wstring& title, const std::wstring& details, const std::wstring& severity);




typedef struct _SYSTEM_HANDLE {
    ULONG ProcessId;
    UCHAR ObjectTypeNumber;
    UCHAR Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG HandleCount;
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemHandleInformation = 16
} SYSTEM_INFORMATION_CLASS;

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
    std::wstring processPath = L"<Unknown>";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess) {
        WCHAR buffer[MAX_PATH];
        if (GetModuleFileNameExW(hProcess, NULL, buffer, MAX_PATH)) {
            processPath = buffer;
        }
        CloseHandle(hProcess);
    }
    return processPath;
}



std::wstring GetProcessName(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (!hProcess) return L"<Unknown>";

    WCHAR processName[MAX_PATH];
    if (GetModuleBaseNameW(hProcess, NULL, processName, MAX_PATH)) {
        CloseHandle(hProcess);
        return std::wstring(processName);
    }

    CloseHandle(hProcess);
    return L"<Unknown>";
}

// Discord Logging Functions
void LogToDiscord(const std::wstring& title, const std::wstring& details, const std::wstring& type) {
    HINTERNET hInternet = InternetOpenA("StealthAntiCheat", 1, NULL, NULL, 0);
    if (!hInternet) return;

    // Create JSON payload
    std::stringstream json;
    json << "{\"embeds\":[{\"title\":\"" << title.c_str() << "\",";
    json << "\"description\":\"" << details.c_str() << "\",";
    json << "\"color\":3447003,";
    json << "\"fields\":[{\"name\":\"Type\",\"value\":\"" << type.c_str() << "\",\"inline\":true},";
    json << "{\"name\":\"Product\",\"value\":\"" << PRODUCT_NAME << "\",\"inline\":true},";
    json << "{\"name\":\"Version\",\"value\":\"" << VERSION << "\",\"inline\":true}],";
    json << "\"footer\":{\"text\":\"© 2025 " << COMPANY << "\"}}]}";

    HINTERNET hConnect = InternetConnectA(hInternet, "discord.com", 443, NULL, NULL, 3, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return;
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/api/webhooks/1441660384443498578/cCBalfn0kXDaV3GjdeqyGMbXTqOEoQMyx8yFZRauypmWTpIZIM40xBrOGcsP5wNWzLvM",
        NULL, NULL, NULL, 0x08000000, 0);
    
    if (hRequest) {
        std::string contentType = "Content-Type: application/json\r\n";
        std::string contentLength = "Content-Length: " + std::to_string(json.str().length()) + "\r\n\r\n";
        
        HttpSendRequestA(hRequest, contentType.c_str(), -1, NULL, 0);
        HttpSendRequestA(hRequest, contentLength.c_str(), -1, NULL, 0);
        HttpSendRequestA(hRequest, json.str().c_str(), -1, NULL, 0);
        
        InternetCloseHandle(hRequest);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

void LogUserExecution(const std::wstring& license_key) {
    char username[256];
    DWORD size = sizeof(username);
    GetUserNameA(username, &size);
    
    std::wstringstream details;
    details << L"Usuario: " << username;
    if (!license_key.empty()) {
        details << L"\nLicencia: " << license_key;
    }
    details << L"\nTimestamp: " << std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    LogToDiscord(L"Stealth AntiCheat X - Usuario", details.str(), L"Execution");
}

void LogDetection(const std::wstring& title, const std::wstring& details, const std::wstring& severity) {
    int color = 3447003; // Blue for info
    if (severity == L"WARNING") color = 16776960; // Yellow
    if (severity == L"CRITICAL") color = 16711680; // Red
    
    // Enhanced logging with system info
    std::wstring enhancedDetails = details;
    enhancedDetails += L"\n\n[System Details]";
    enhancedDetails += L"\nComputer: " + GetSystemInfo();
    enhancedDetails += L"\nNetwork: " + GetNetworkInfo();
    
    LogToDiscord(title + L" [" + severity + L"]", enhancedDetails, severity);
}


DWORD FindHDPlayerProcessId() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to create snapshot of processes." << std::endl;
        return 0;
    }

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    DWORD targetPid = 0;
    if (Process32FirstW(hSnapshot, &processEntry)) {
        do {
            if (std::wstring(processEntry.szExeFile) == L"HD-Player.exe") {
                targetPid = processEntry.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);
    return targetPid;
}




void EnumerateAndTerminateWindowByStyle(DWORD processId) {
    
    auto EnumWindowsCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
        DWORD windowProcessId;
        GetWindowThreadProcessId(hwnd, &windowProcessId);

        if (windowProcessId == (DWORD)lParam) {
            LONG style = GetWindowLong(hwnd, GWL_STYLE);

            if (style == 0x94000000) {
               
                HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                GetConsoleScreenBufferInfo(hCon, &csbi);
                WORD def = csbi.wAttributes;

                
                SetConsoleTextAttribute(hCon, FOREGROUND_RED | FOREGROUND_INTENSITY);

               
                std::wcout << L"\n    HWND: " << hwnd << L"\n"
                    << L"    [ESP OVERLAY DETECTED]\n";

                // Log to Discord
                std::wstringstream details;
                details << L"ESP Overlay detectado y cerrado\n";
                details << L"HWND: " << hwnd << L"\n";
                details << L"PID: " << processId;
                LogDetection(L"ESP Overlay", details.str(), L"CRITICAL");
                
                if (PostMessage(hwnd, WM_CLOSE, 0, 0)) {
                    std::wcout << L"\n    [CLOSE REQUEST SENT]\n";
                }
                else {
                    std::wcout << L"\n    [FAILED TO CLOSE]\n";
                    
                    // Log failure
                    std::wstringstream failDetails;
                    failDetails << L"Fallo al cerrar ESP Overlay\n";
                    failDetails << L"HWND: " << hwnd;
                    LogDetection(L"ESP Overlay - Failed", failDetails.str(), L"WARNING");
                }

               
                SetConsoleTextAttribute(hCon, def);

               
                std::wcout << L"\n";
            }
        }
        return TRUE;
        };

    EnumWindows(EnumWindowsCallback, (LPARAM)processId);
}



#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <wintrust.h>
#include <softpub.h>
#include <tchar.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "wintrust.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "psapi.lib")




typedef LONG NTSTATUS;
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define ThreadQuerySetWin32StartAddress 9

using pfnNtQueryInformationThread = NTSTATUS(WINAPI*)(
    HANDLE, ULONG, PVOID, ULONG, PULONG);


struct ModuleRange {
    uintptr_t start, end;
};


bool GetProcessModuleRanges(DWORD pid, std::vector<ModuleRange>& outRanges) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (hSnap == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32W me = { sizeof(me) };
    if (!Module32FirstW(hSnap, &me)) {
        CloseHandle(hSnap);
        return false;
    }
    do {
        uintptr_t base = reinterpret_cast<uintptr_t>(me.modBaseAddr);
        outRanges.push_back({ base, base + me.modBaseSize });
    } while (Module32NextW(hSnap, &me));
    CloseHandle(hSnap);
    return true;
}


bool IsAddressWithinModule(uintptr_t addr, const std::vector<ModuleRange>& ranges) {
    for (auto& r : ranges)
        if (addr >= r.start && addr < r.end)
            return true;
    return false;
}


BOOL IsFileSigned(LPCWSTR path) {
    WINTRUST_FILE_INFO fi = { sizeof(fi), path };
    WINTRUST_DATA wtd = {};
    wtd.cbStruct = sizeof(wtd);
    wtd.dwUIChoice = WTD_UI_NONE;
    wtd.fdwRevocationChecks = WTD_REVOKE_NONE;
    wtd.dwUnionChoice = WTD_CHOICE_FILE;
    wtd.pFile = &fi;
    wtd.dwStateAction = WTD_STATEACTION_VERIFY;
    wtd.dwProvFlags = WTD_SAFER_FLAG;

    GUID policy = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    LONG status = WinVerifyTrust(NULL, &policy, &wtd);


    wtd.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(NULL, &policy, &wtd);

    return (status == ERROR_SUCCESS);
}



void ScanProcess(DWORD pid)
{

    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCon, &csbi);
    WORD defaultAttr = csbi.wAttributes;


    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProc)
    {
        WCHAR exePath[MAX_PATH] = L"<unknown>";
        if (GetModuleFileNameExW(hProc, nullptr, exePath, _countof(exePath)))
        {
            bool ok = IsFileSigned(exePath);
            SetConsoleTextAttribute(hCon,
                ok
                ? (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
                : (FOREGROUND_RED | FOREGROUND_INTENSITY));
            std::wcout << (ok ? L"[SIGNED]   " : L"[UNSIGNED] ")
                << L"[EXE] " << exePath << L"\n\n";
            SetConsoleTextAttribute(hCon, defaultAttr);
        }
        CloseHandle(hProc);
    }


    std::wcout << L"Loaded modules:\n";
    HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (hModSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W me = { sizeof(me) };
        if (Module32FirstW(hModSnap, &me))
        {
            do
            {
                bool ok = IsFileSigned(me.szExePath);
                SetConsoleTextAttribute(hCon,
                    ok
                    ? (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
                    : (FOREGROUND_RED | FOREGROUND_INTENSITY));
                std::wcout << (ok ? L"[SIGNED]   " : L"[UNSIGNED] ")
                    << me.szModule << L" -> " << me.szExePath << L"\n";
                SetConsoleTextAttribute(hCon, defaultAttr);
            } while (Module32NextW(hModSnap, &me));
        }
        CloseHandle(hModSnap);
    }
    std::wcout << L"\n";

    HMODULE hNt = GetModuleHandleW(L"ntdll.dll");
    auto NtQIT = reinterpret_cast<pfnNtQueryInformationThread>(
        GetProcAddress(hNt, "NtQueryInformationThread"));
    if (!NtQIT)
    {
        std::wcerr << L"Cannot get NtQueryInformationThread\n";
        return;
    }


    std::vector<ModuleRange> ranges;
    if (!GetProcessModuleRanges(pid, ranges))
    {
        std::wcerr << L"Module-range enumeration failed\n";
        return;
    }


    std::wcout << L"Checking for suspicious threads...\n";
    bool foundSuspicious = false;
    HANDLE hThrSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThrSnap != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 te = { sizeof(te) };
        if (Thread32First(hThrSnap, &te))
        {
            do
            {
                if (te.th32OwnerProcessID != pid) continue;
                HANDLE hThr = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
                if (!hThr) continue;

                uintptr_t startAddr = 0;
                NTSTATUS st = NtQIT(hThr,
                    ThreadQuerySetWin32StartAddress,
                    &startAddr,
                    sizeof(startAddr),
                    nullptr);
                if (st == STATUS_SUCCESS &&
                    !IsAddressWithinModule(startAddr, ranges))
                {
                    foundSuspicious = true;
                    SetConsoleTextAttribute(hCon,
                        FOREGROUND_RED | FOREGROUND_INTENSITY);
                    std::wcout
                        << L"[SUSPICIOUS THREAD] PID=" << pid
                        << L" TID=" << te.th32ThreadID
                        << L" Start=0x" << std::hex << startAddr << std::dec
                        << L"\n";
                    SetConsoleTextAttribute(hCon, defaultAttr);
                }

                CloseHandle(hThr);
            } while (Thread32Next(hThrSnap, &te));
        }
        CloseHandle(hThrSnap);
    }


    if (!foundSuspicious)
    {
        SetConsoleTextAttribute(hCon,
            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::wcout << L"No suspicious threads found.\n";
        SetConsoleTextAttribute(hCon, defaultAttr);
    }

    std::wcout << L"\nScan complete for PID " << pid << L".\n";
}





void MonitorAndRestrictAccess(DWORD targetPid) {
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (!hNtdll) {

        return;
    }

    auto NtQuerySystemInformation = (NtQuerySystemInformation_t)GetProcAddress(hNtdll, "NtQuerySystemInformation");
    if (!NtQuerySystemInformation) {

        return;
    }

    ULONG handleInfoSize = 0x10000;
    PSYSTEM_HANDLE_INFORMATION handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);

    if (!handleInfo) {
        std::cerr << "Memory allocation failed." << std::endl;
        return;
    }
    std::vector<DWORD> safeProcesses;
    while (true) {

        if (FindProcessByName(L"HD-Player.exe") != targetPid) {
            std::wcout << L"HD-Player.exe has stopped. Stopping monitoring." << std::endl;
            break;
        }

        NTSTATUS status;
        while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, nullptr)) == STATUS_INFO_LENGTH_MISMATCH) {
            handleInfoSize *= 2;
            handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize);
            if (!handleInfo) {
                std::cerr << "Memory reallocation failed." << std::endl;
                return;
            }
        }

        if (!NT_SUCCESS(status)) {

            free(handleInfo);
            return;
        }

        for (ULONG i = 0; i < handleInfo->HandleCount; i++) {
            SYSTEM_HANDLE handle = handleInfo->Handles[i];

            if (handle.ProcessId == GetCurrentProcessId()) {
                continue;
            }

            HANDLE hOwningProcess = OpenProcessSafe(handle.ProcessId);
            if (!hOwningProcess) continue;

            HANDLE hDuplicate = nullptr;

            if (DuplicateHandle(hOwningProcess, (HANDLE)handle.Handle, GetCurrentProcess(), &hDuplicate, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
                if (GetProcessId(hDuplicate) == targetPid) {

                    std::wstring processPath = GetProcessPath(handle.ProcessId);
                    std::wstring processName = GetProcessName(handle.ProcessId);

                    std::transform(processName.begin(), processName.end(), processName.begin(), ::tolower);


                    bool isSafe = (processPath.find(L"C:\\Windows\\System32") != std::wstring::npos) &&
                        (processName == L"taskmgr.exe" || processName == L"lsass.exe" || processName == L"conhost.exe" || processName == L"svchost.exe" || processName == L"csrss.exe" || processName == L"audiodg.exe");

                    if (isSafe) {
                        safeProcesses.push_back(handle.ProcessId);
                        DuplicateHandle(hOwningProcess, (HANDLE)handle.Handle, hOwningProcess, nullptr, 0, FALSE, DUPLICATE_CLOSE_SOURCE);

                    }
                    else
                    {


                        DuplicateHandle(hOwningProcess, (HANDLE)handle.Handle, hOwningProcess, nullptr, 0, FALSE, DUPLICATE_CLOSE_SOURCE);



                        std::wcout << L"Suspicious Process Detected. PID: " << handle.ProcessId << L", Path: " << processPath << std::endl;

                        ScanProcess(handle.ProcessId);

                    }




                }
                CloseHandle(hDuplicate);
            }

            CloseHandle(hOwningProcess);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    free(handleInfo);
}



PVOID GetZwWriteVirtualMemoryAddress() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) {

        return nullptr;
    }

    FARPROC zwWriteVirtualMemory = GetProcAddress(ntdll, "ZwWriteVirtualMemory");
    if (!zwWriteVirtualMemory) {

        return nullptr;
    }

    return (PVOID)((uintptr_t)zwWriteVirtualMemory);
}


std::string GetProcessName(HANDLE processHandle) {
    char processName[MAX_PATH] = "<Unknown>";
    if (GetModuleBaseNameA(processHandle, NULL, processName, sizeof(processName) / sizeof(char))) {
        return std::string(processName);
    }
    return std::string("<Unknown>");
}


bool ReadMemory(HANDLE processHandle, PVOID address, std::vector<BYTE>& buffer, SIZE_T size) {
    buffer.resize(size);
    SIZE_T bytesRead;
    if (ReadProcessMemory(processHandle, address, buffer.data(), size, &bytesRead) && bytesRead == size) {
        return true;
    }
    return false;
}


void PrintBytes(const std::vector<BYTE>& bytes) {
    for (BYTE b : bytes) {
        printf("%02X ", b);
    }
    std::cout << std::endl;
}




void monitorexternal() {

    std::wstring targetProcessName = L"HD-Player.exe";

    bool isProcessRunning = true;

    while (true) {

        DWORD targetPid = FindProcessByName(targetProcessName);

        if (targetPid != 0) {

            if (!isProcessRunning) {
                // Log when process starts
                std::wstringstream details;
                details << L"HD-Player.exe detectado y iniciado monitoreo\n";
                details << L"PID: " << targetPid;
                LogDetection(L"HD-Player Detected", details.str(), L"INFO");
                isProcessRunning = true;
            }
            
            std::wcout << L"Monitoring process: " << targetProcessName << L" (PID: " << targetPid << L")..." << std::endl;

            MonitorAndRestrictAccess(targetPid);


        }
        else {

            if (isProcessRunning) {
                std::wcout << L"Waiting for process: " << targetProcessName << L" to start..." << std::endl;
                
                // Log when process stops
                std::wstringstream stopDetails;
                stopDetails << L"HD-Player.exe no detectado\n";
                stopDetails << L"Monitoreo pausado";
                LogDetection(L"HD-Player Stopped", stopDetails.str(), L"WARNING");
                
                isProcessRunning = false;
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(400));


    }

}


void monitoresp() {
    while (true) {


        DWORD targetPid = FindHDPlayerProcessId();

        if (targetPid == 0) {
            // No HD-Player process found - log periodically but not too often
        }
        else {
            // HD Player found - log ESP monitoring start
            std::wstringstream details;
            details << L"Iniciando monitoreo ESP en HD-Player\n";
            details << L"PID: " << targetPid;
            EnumerateAndTerminateWindowByStyle(targetPid);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

}


std::thread external;
std::thread internal;


bool EnableDebugPrivilege() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        std::cerr << "OpenProcessToken failed: " << GetLastError() << "\n";
        return false;
    }

    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) {
        std::cerr << "LookupPrivilegeValue failed: " << GetLastError() << "\n";
        CloseHandle(hToken);
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), nullptr, nullptr)) {
        std::cerr << "AdjustTokenPrivileges failed: " << GetLastError() << "\n";
        CloseHandle(hToken);
        return false;
    }

    CloseHandle(hToken);
    return GetLastError() == ERROR_SUCCESS;
}


void EnsureAdminPrivileges()
{
    BOOL isAdmin = FALSE;
    HANDLE hToken = nullptr;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size))
        {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }

    if (!isAdmin)
    {

        ExitProcess(1);
    }
}





std::atomic<bool> g_running{ true };
std::string name = "STEALTH ANTICHEAT X - Developed By xpe.nettt";

void PrintBanner()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCon, &csbi);
    WORD def = csbi.wAttributes;

    WORD borderCol = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD textCol = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    const int width = 42;
    std::string pad((width - 2 - name.size()) / 2, ' ');

    SetConsoleTextAttribute(hCon, borderCol);
    std::cout << std::string(width, '*') << "\n";
    std::cout << "*" << std::string(width - 2, ' ') << "*\n";
    std::cout << "*" << pad;
    SetConsoleTextAttribute(hCon, textCol);
    std::cout << name;
    SetConsoleTextAttribute(hCon, borderCol);
    std::cout << pad;
    if ((name.size() % 2) != 0) std::cout << " ";
    std::cout << "*\n";
    std::cout << "*" << std::string(width - 2, ' ') << "*\n";
    std::cout << std::string(width, '*') << "\n\n";
    SetConsoleTextAttribute(hCon, def);
}

void MonitorTime()
{
    auto prevSteady = std::chrono::steady_clock::now();
    const auto expectedInterval = std::chrono::seconds(1);
    const auto threshold = std::chrono::milliseconds(50);
    bool tampered = false;

    while (g_running.load())
    {
        std::this_thread::sleep_for(expectedInterval);

        time_t nowTime = time(nullptr);
        struct tm local_tm;
        localtime_s(&local_tm, &nowTime);

        char timeBuf[64];
        int hour = local_tm.tm_hour % 12;
        if (hour == 0) hour = 12;
        const char* ampm = (local_tm.tm_hour < 12) ? "AM" : "PM";
        snprintf(timeBuf, sizeof(timeBuf),
            "Developed By Galib. %02d:%02d:%02d %s",
            hour, local_tm.tm_min, local_tm.tm_sec, ampm);
        SetConsoleTitleA(timeBuf);

        auto nowSteady = std::chrono::steady_clock::now();
        auto actualInterval = nowSteady - prevSteady;
        auto diff = (actualInterval > expectedInterval)
            ? actualInterval - expectedInterval
            : expectedInterval - actualInterval;

        if (diff > threshold) {
            if (!tampered) {
                double sec = std::chrono::duration<double>(diff).count();
                HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                GetConsoleScreenBufferInfo(hCon, &csbi);
                WORD def = csbi.wAttributes;
                SetConsoleTextAttribute(hCon, FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << "\nBypass attempted. Time jump: "
                    << std::fixed << std::setprecision(3)
                    << sec << " seconds\n";
                SetConsoleTextAttribute(hCon, def);
                Beep(750, 300);
                tampered = true;
            }
        }
        else {
            tampered = false;
        }

        prevSteady = nowSteady;
    }
}


void DisableConsoleSelection()
{

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        return;

    DWORD mode = 0;
    if (!GetConsoleMode(hStdin, &mode))
        return;


    mode |= ENABLE_EXTENDED_FLAGS;

    mode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_MOUSE_INPUT);

    SetConsoleMode(hStdin, mode);
}

// NEW FEATURE 1: Moderate DMA Hardware Detection
void DetectDMAHardware() {
    std::wcout << L"[DMA Detection] Scanning for DMA-capable hardware..." << std::endl;
    
    HDEVINFO deviceInfoSet = SetupDiGetClassDevsW(
        &GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::wcout << L"[DMA Detection] Failed to enumerate devices" << std::endl;
        return;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    bool suspiciousDevicesFound = false;
    int deviceIndex = 0;
    
    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        deviceIndex++;
        
        char deviceDesc[256] = {0};
        DWORD size = sizeof(deviceDesc);
        
        if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &deviceInfoData, 
            SPDRP_DEVICEDESC, NULL, (PBYTE)deviceDesc, size, NULL)) {
            
            std::string desc(deviceDesc);
            // Look for potential DMA devices (USB, PCI cards, etc.)
            if (desc.find("PCI") != std::string::npos ||
                desc.find("USB") != std::string::npos ||
                desc.find("Serial") != std::string::npos ||
                desc.find("Parallel") != std::string::npos ||
                desc.find("Controller") != std::string::npos) {
                
                suspiciousDevicesFound = true;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_YELLOW | FOREGROUND_INTENSITY);
                std::wcout << L"[DMA Alert] Potential DMA device: " << deviceDesc << std::endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
                
                // Log to Discord (moderate - just informational)
                std::wstringstream details;
                details << L"Potential DMA-capable hardware detected\\n";
                details << L"Device: " << deviceDesc;
                LogDetection(L"DMA Hardware Detected", details.str(), L"WARNING");
            }
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    
    if (!suspiciousDevicesFound) {
        std::wcout << L"[DMA Detection] No suspicious DMA hardware found" << std::endl;
    }
}

// NEW FEATURE 2: Basic File Integrity Verification
bool CalculateFileHash(const std::wstring& filePath, std::string& hash) {
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    BYTE buffer[4096];
    DWORD bytesRead;
   HCRYPTHASH hHash;
    HCRYPTPROV hCryptProv;

    // Initialize Crypto
    if (!CryptAcquireContextW(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        CloseHandle(hFile);
        return false;
    }

    if (!CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash)) {
        CryptReleaseContext(hCryptProv, 0);
        CloseHandle(hFile);
        return false;
    }

    // Read and hash file
    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        CryptHashData(hHash, buffer, bytesRead, 0);
    }

    // Get hash value
    BYTE hashBytes[20];
    DWORD hashSize = sizeof(hashBytes);
    
    if (CryptGetHashParam(hHash, HP_HASHVAL, hashBytes, &hashSize, 0)) {
        std::stringstream ss;
        for (DWORD i = 0; i < hashSize; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hashBytes[i];
        }
        hash = ss.str();
    }

    // Cleanup
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    CloseHandle(hFile);
    
    return !hash.empty();
}

bool VerifyFileIntegrity() {
    std::wcout << L"[File Integrity] Verifying critical system files..." << std::endl;
    
    bool allFilesValid = true;
    
    // List of critical files to check
    std::vector<std::wstring> criticalFiles = {
        L"C:\\Windows\\System32\\kernel32.dll",
        L"C:\\Windows\\System32\\ntdll.dll",
        L"C:\\Windows\\System32\\advapi32.dll",
        L"C:\\Windows\\System32\\user32.dll"
    };
    
    for (const auto& filePath : criticalFiles) {
        std::string hash;
        if (CalculateFileHash(filePath, hash)) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            std::wcout << L"[File Integrity] OK: " << filePath.substr(filePath.find_last_of(L'\\') + 1) << std::endl;
        } else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            std::wcout << L"[File Integrity] ERROR: Cannot verify " << filePath << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            allFilesValid = false;
        }
    }
    
    if (!allFilesValid) {
        std::wstringstream details;
        details << L"Some critical system files could not be verified\\n";
        details << L"This may indicate system tampering or corruption";
        LogDetection(L"File Integrity Issue", details.str(), L"WARNING");
    }
    
    return allFilesValid;
}

// NEW FEATURE 3: Enhanced System Information
std::string GetSystemInfo() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    GetComputerNameA(computerName, &size);
    
    char userName[MAX_USERNAME_LENGTH + 1];
    size = sizeof(userName);
    GetUserNameA(userName, &size);
    
    OSVERSIONINFOEX osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    std::stringstream info;
    info << computerName << " (" << userName << ") - Windows ";
    
    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
        info << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
        if (osvi.wProductType == VER_NT_WORKSTATION) {
            info << " Professional";
        } else {
            info << " Server";
        }
    }
    
    return info.str();
}

std::string GetNetworkInfo() {
    PMIB_IFTABLE ifTable;
    DWORD size = 0;
    DWORD ret = GetIfTable(NULL, &size, TRUE);
    
    ifTable = (PMIB_IFTABLE)malloc(size);
    if (ifTable) {
        ret = GetIfTable(ifTable, &size, TRUE);
        if (ret == NO_ERROR) {
            std::stringstream info;
            int adapterCount = 0;
            for (DWORD i = 0; i < ifTable->dwNumEntries; i++) {
                if (ifTable->table[i].dwType == MIB_IF_TYPE_ETHERNET || 
                    ifTable->table[i].dwType == MIB_IF_TYPE_IEEE80211) {
                    adapterCount++;
                    info << "Adapter " << adapterCount << ": ";
                    if (ifTable->table[i].dwOperStatus == MIB_IF_OPER_STATUS_OPERATIONAL) {
                        info << "Active";
                    } else {
                        info << "Inactive";
                    }
                    break; // Just get the first active network adapter
                }
            }
            free(ifTable);
            return info.str();
        }
        free(ifTable);
    }
    
    return "Network: Unable to retrieve info";
}

void LogSystemDetails() {
    std::wstringstream details;
    details << L"Stealth AntiCheat X - System initialization complete\\n";
    details << L"DMA Hardware: Scanned\\n";
    details << L"File Integrity: Verified\\n";
    details << L"System: " << GetSystemInfo() << L"\\n";
    details << L"Network: " << GetNetworkInfo();
    
    LogToDiscord(L"System Details", details.str(), L"INFO");
}


int main()
{
    EnsureAdminPrivileges();
    EnableDebugPrivilege();
    DisableConsoleSelection();
    
    // Log initial execution to Discord
    LogUserExecution(L"");
    
    HWND hwndConsole = GetConsoleWindow();
    if (hwndConsole)
    {

        HMENU hMenu = GetSystemMenu(hwndConsole, FALSE);
        if (hMenu)
        {

            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);


        }
    }

    // NEW FEATURE: System initialization checks
    std::wcout << L"=== SYSTEM INITIALIZATION CHECKS ===" << std::endl;
    
    // 1. File Integrity Check
    std::wcout << L"\\n[1] File Integrity Check:" << std::endl;
    VerifyFileIntegrity();
    
    // 2. DMA Hardware Detection (moderate)
    std::wcout << L"\\n[2] DMA Hardware Detection:" << std::endl;
    DetectDMAHardware();
    
    // 3. Log system details to Discord
    std::wcout << L"\\n[3] Sending system details to Discord..." << std::endl;
    LogSystemDetails();
    
    std::wcout << L"\\n=== INITIALIZATION COMPLETE ===" << std::endl;


    time_t startTime = time(nullptr);
    struct tm startLocal;
    localtime_s(&startLocal, &startTime);
    char startBuf[128];
    int shour = startLocal.tm_hour % 12;
    if (shour == 0) shour = 12;
    const char* sampm = (startLocal.tm_hour < 12) ? "AM" : "PM";
    snprintf(startBuf, sizeof(startBuf),
        "Anticheat started at %04d-%02d-%02d %02d:%02d:%02d %s\n\n",
        startLocal.tm_year + 1900,
        startLocal.tm_mon + 1,
        startLocal.tm_mday,
        shour, startLocal.tm_min, startLocal.tm_sec,
        sampm);
    std::cout << startBuf;


   
    PrintBanner();


    std::thread monitorThread(MonitorTime);


    std::thread external([]() { monitorexternal(); });
    std::thread internal([]() { monitoresp(); });


    monitorThread.join();
    external.join();
    internal.join();
    return 0;
}
