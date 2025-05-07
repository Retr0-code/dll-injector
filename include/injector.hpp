#pragma once
#include <string>
#include <windows.h>

class DLLInjector {
public:
#ifdef USE_WFUNCTIONS
    DLLInjector(DWORD32 dw_pid, LPCWSTR lpcs_dllPath);
#else
    DLLInjector(DWORD32 dw_pid, LPCSTR lpcs_dllPath);
#endif

    ~DLLInjector();

    void Inject(void);

private:
    bool EnableDebugPrivilege(void);

private:
#ifdef USE_WFUNCTIONS
    std::wstring _sw_dllPath;
#else
    std::string _sw_dllPath;
#endif
    HANDLE _h_process;
    LPVOID _lpv_dllAddress;
    DWORD32 _dw_pid;
};
