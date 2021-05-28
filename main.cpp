#include <Windows.h>
#include <wchar.h>

bool SetVariable(LPCTSTR name, LPCTSTR value, bool global)
{
    HKEY key = global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    LPCWSTR keyPath = global ? L"System\\CurrentControlSet\\Control\\Session Manager\\Environment" : L"Environment";

    HKEY hKey;
    LSTATUS keyStatus = RegOpenKeyEx(key, keyPath, 0, KEY_ALL_ACCESS, &hKey);

    if (keyStatus == ERROR_SUCCESS)
    {
        size_t valueSize = (wcslen(value) + 1) * sizeof(wchar_t);
        LSTATUS setStatus = RegSetValueEx(hKey, name, 0, REG_SZ, (LPBYTE)value, valueSize);
        RegCloseKey(hKey);

        if (setStatus == ERROR_SUCCESS)
        {
            SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment");
            return true;
        }
    }

    return false;
}

int PrintUsage()
{
    wprintf(L"Set user variable:\n");
    wprintf(L"\tenvmod.exe /u <name> <value>\n");
    wprintf(L"\n");
    wprintf(L"Set system variable:\n");
    wprintf(L"\tenvmod.exe /s <name> <value>\n");
    return 0;
}


int wmain(int argc, wchar_t* argv[])
{
    if (argc < 4)
    {
        PrintUsage();
        return 0;
    }

    bool global;
    if (wcscmp(argv[1], L"/u") == 0)
        global = false;
    else if(wcscmp(argv[1], L"/s") == 0)
        global = true;
    else
    {
        PrintUsage();
        return 0;
    }

    if (SetVariable(argv[2], argv[3], global))
        wprintf(L"Variable \"%s\" set ok", argv[2]);
    else
        wprintf(L"Variable \"%s\" set failed", argv[2]);

    return 0;
}