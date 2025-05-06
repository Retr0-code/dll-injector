#include <memory>
#include <iostream>
#include <windows.h>

#include "injector.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <PID> <C:/path/to/DLL>\n";
        return -1;
    }

    DWORD dw_pid{std::atoi(argv[1])};
    if (!dw_pid) {
        std::cerr << "Error: Invalid PID=" << dw_pid << '\n';
        return -1;
    }

    std::unique_ptr<DLLInjector> dllInjector{nullptr};
    try {
        dllInjector = std::make_unique<DLLInjector>(dw_pid, argv[2]);
    } catch(const std::exception &error) {
        std::cerr << error.what();
        return -1;
    }

    try {
        dllInjector->Inject();
    } catch(const std::exception &error) {
        std::cerr << error.what();
        return -1;
    }

    std::cout << "Successfully injected DLL\n";

    return 0;
}
