#include <windows.h>

// a sample exported function
void MsgBox(const char *message) {
    MessageBoxA(NULL, message, "Injected DLL", MB_OK);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            MsgBox("DLL_PROCESS_ATTACH");
            break;

        case DLL_PROCESS_DETACH:
            MsgBox("DLL_PROCESS_DETACH");
            break;

        case DLL_THREAD_ATTACH:
            MsgBox("DLL_THREAD_ATTACH");
            break;

        case DLL_THREAD_DETACH:
            MsgBox("DLL_THREAD_DETACH");
            break;
    }
    return TRUE;
}
