#include <sstream>
#include <stdexcept>

#include "injector.hpp"

DLLInjector::DLLInjector(DWORD32 dw_pid, LPCSTR lpcs_dllPath)
: _sa_dllPath(lpcs_dllPath), _dw_pid(dw_pid) {
    
    DWORD dw_procAttr = ::GetFileAttributesA(lpcs_dllPath);
    if(dw_procAttr == INVALID_FILE_ATTRIBUTES || (dw_procAttr & FILE_ATTRIBUTE_DIRECTORY)) {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Invalid DLL path=" << this->_sa_dllPath << '\n';
        throw std::invalid_argument(sv_errorMessage.str());
    }

    if (!this->EnableDebugPrivilege()) {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Unable to gain debug privilege\n"
            << std::system_category().message(::GetLastError()) << '\n';
        throw std::runtime_error(sv_errorMessage.str());
    }
    
    this->_h_process = ::OpenProcess(PROCESS_ALL_ACCESS, false, dw_pid);
    // this->_h_process = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, false, dw_pid);
    if (this->_h_process == NULL) {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Unable to open process handle\n"
            << std::system_category().message(::GetLastError()) << '\n';
        throw std::runtime_error(sv_errorMessage.str());
    }
}

DLLInjector::~DLLInjector() {
    ::CloseHandle(this->_h_process);
}

void DLLInjector::Inject(void) {
    // Retrieves pointer to LoadLibraryA(scii) in KERNEL32.DLL
    LPVOID lpv_loadLibrary = (LPVOID)::GetProcAddress(
        ::GetModuleHandleA("KERNEL32.dll"), "LoadLibraryA");
    
    if (lpv_loadLibrary == NULL)
    {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Unable to get KERNEL32.dll!LoadLibraryA address\n"
            << std::system_category().message(::GetLastError()) << '\n';
        throw std::runtime_error(sv_errorMessage.str());
    }

    // Allocates remote processes memory for new DLL
    LPVOID lpv_dllAddress = (LPVOID)::VirtualAllocEx(
        this->_h_process, nullptr, this->_sa_dllPath.length(),
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    if (lpv_dllAddress == NULL) {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Unable to allocate memory for DLL\n"
            << std::system_category().message(::GetLastError()) << '\n';
        throw std::runtime_error(sv_errorMessage.str());
    }

    if (::WriteProcessMemory(this->_h_process, lpv_dllAddress,
        this->_sa_dllPath.c_str(), this->_sa_dllPath.length(), 0) == 0) {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Unable to write load DLL\n"
            << std::system_category().message(::GetLastError()) << '\n';
        throw std::runtime_error(sv_errorMessage.str());
    }

    if (::CreateRemoteThread(this->_h_process, 0, 0, (LPTHREAD_START_ROUTINE)lpv_loadLibrary,
        (LPVOID)lpv_dllAddress, 0, 0) == NULL) {
        std::stringstream sv_errorMessage;
        sv_errorMessage << "Error: Unable to launch DLL thread\n"
            << std::system_category().message(::GetLastError()) << '\n';
        throw std::runtime_error(sv_errorMessage.str());
    }
}

bool DLLInjector::EnableDebugPrivilege(void) {
    HANDLE h_token;
    LUID luid;
    TOKEN_PRIVILEGES tokenPrivileges;
    bool status = true;

    while (true) {
        if (!::OpenProcessToken(::GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token)) {
            status = false;
            break;
        }
        
        if (!::LookupPrivilegeValueA(NULL, SE_DEBUG_NAME, &luid)) {
            status = false;
            break;
        }
        
        tokenPrivileges.PrivilegeCount = 1;
        tokenPrivileges.Privileges[0].Luid = luid;
        tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (!::AdjustTokenPrivileges(h_token, false, &tokenPrivileges, sizeof(tokenPrivileges), NULL, NULL))
            status = false;

        break;
    }
    ::CloseHandle(h_token);
    return status;
}
