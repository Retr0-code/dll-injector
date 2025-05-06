#pragma once
#include <string>
#include <windows.h>

class DLLInjector {
public:
    DLLInjector(DWORD32 dw_pid, LPCSTR lpcs_dllPath);

    ~DLLInjector();

    void Inject(void);

private:
    bool EnableDebugPrivilege(void);

private:
    std::string _sa_dllPath;
    DWORD32 _dw_pid;
    HANDLE _h_process;
};
