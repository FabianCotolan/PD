#include <windows.h>
#include <winreg.h>
#include <iostream>

#define SERVICES_PATH "SYSTEM\\CurrentControlSet\\Services"

void ReadImagePath(HKEY hRootKey, LPCSTR subKeyPath) {
    HKEY hKey;
    if (RegOpenKeyExA(hRootKey, subKeyPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cerr << "Eroare la deschiderea cheii Registry!" << std::endl;
        return;
    }

    char subKeyName[256];
    DWORD subKeySize;
    DWORD index = 0;
    DWORD subKeyCount;

    
    RegQueryInfoKeyA(hKey, NULL, NULL, NULL, &subKeyCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    std::cout << "Subchei gasite: " << subKeyCount << std::endl;

    
    while (index < subKeyCount) {
        subKeySize = sizeof(subKeyName);
        if (RegEnumKeyExA(hKey, index, subKeyName, &subKeySize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
            break;

       
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", subKeyPath, subKeyName);

        HKEY hSubKey;
        if (RegOpenKeyExA(hRootKey, fullPath, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
            char imagePath[1024] = { 0 };
            DWORD pathSize = sizeof(imagePath);
            DWORD type;

            
            if (RegQueryValueExA(hSubKey, "ImagePath", NULL, &type, (LPBYTE)imagePath, &pathSize) == ERROR_SUCCESS) {
                if (type == REG_EXPAND_SZ || type == REG_SZ) {
                    std::cout << "[" << subKeyName << "] ImagePath: " << imagePath << std::endl;
                }
            }
            RegCloseKey(hSubKey);
        }
        index++;
    }

    RegCloseKey(hKey);
}

int main() {
    std::cout << "Cautare ImagePath in " SERVICES_PATH "...\n";
    ReadImagePath(HKEY_LOCAL_MACHINE, SERVICES_PATH);
    return 0;
}
