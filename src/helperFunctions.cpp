#include "HelperFunctions.h"
#include "ConfigFile.h"
#include "Limit.h"
#include <algorithm>
#include <psapi.h>
#include <Windows.h>

void Helper::ExitApp(bool debug)
{
    std::cout << "shutting down" << std::endl;
    ShellExecute(nullptr, nullptr, L"powershell.exe", L"-ExecutionPolicy bypass -c Remove-NetQosPolicy -Name 'Destiny2-Limit' -Confirm:$false", nullptr, SW_HIDE);
    if (!debug) 
    {
        ShowWindow(GetConsoleWindow(), SW_RESTORE);
    }
    PostQuitMessage(0);
}


bool Helper::D2Active()
{
    TCHAR buffer[MAX_PATH] = { 0 };
    DWORD dw_proc_id       = 0;

    GetWindowThreadProcessId(GetForegroundWindow(), &dw_proc_id);

    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dw_proc_id);
    GetModuleFileNameEx(hProc, nullptr, buffer, MAX_PATH);
    CloseHandle(hProc);

    const wchar_t* filename = GetFileName(buffer);
    std::cout << "active window filename: " << filename << std::endl;

    if (wcscmp(filename, L"destiny2.exe") == 0) 
    {
        return true;
    }

    return false;
}


bool Helper::RunningAsAdmin()
{
    bool fRet     = false;
    HANDLE hToken = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) 
    {
        TOKEN_ELEVATION elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &cbSize)) 
        {
            fRet = elevation.TokenIsElevated;
        }
    }
    if (hToken) 
    {
        CloseHandle(hToken);
    }
    return fRet;
}


const wchar_t* Helper::GetFileName(const wchar_t* path)
{
    const wchar_t* filename = wcsrchr(path, '\\');
    if (filename == nullptr) 
    {
        filename = path;
    }
    else 
    {
        filename++;
    }
    return filename;
}
