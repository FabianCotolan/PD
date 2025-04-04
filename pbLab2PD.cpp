#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <usbiodef.h>
#include <devguid.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

int main() {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD index = 0;

    // Obtine toate dispozitivele din categoria USB
    deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB, // Clasa de dispozitive USB
        0,
        0,
        DIGCF_PRESENT // Doar dispozitivele prezente
    );

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Eroare la obtinerea listei de dispozitive." << std::endl;
        return 1;
    }

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    while (SetupDiEnumDeviceInfo(deviceInfoSet, index, &deviceInfoData)) {
        index++;

        TCHAR deviceName[1024];
        if (SetupDiGetDeviceRegistryProperty(
            deviceInfoSet,
            &deviceInfoData,
            SPDRP_DEVICEDESC,
            NULL,
            (PBYTE)deviceName,
            sizeof(deviceName),
            NULL)) {
            std::wcout << L"Dispozitiv USB: " << deviceName << std::endl;
        }
        else {
            std::cerr << "Eroare la citirea descrierii dispozitivului." << std::endl;
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return 0;
}
