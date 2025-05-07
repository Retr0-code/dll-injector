#include <memory>
#include <iostream>
#include <windows.h>

#include "injector.hpp"

#ifdef USE_WFUNCTIONS
int wmain(int argc, wchar_t **argv) {
#else
int main(int argc, char **argv) {
#endif
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <PID> <C:/path/to/DLL>\n";
        return EXIT_FAILURE;
    }

    DWORD dw_pid{std::stoi(argv[1])};
    if (!dw_pid) {
        std::cerr << "Error: Invalid PID=" << dw_pid << '\n';
        return EXIT_FAILURE;
    }

    std::unique_ptr<DLLInjector> dllInjector{nullptr};
    try {
        dllInjector = std::make_unique<DLLInjector>(dw_pid, argv[2]);
    } catch(const std::exception &error) {
        std::cerr << error.what();
        return EXIT_FAILURE;
    }

    try {
        dllInjector->Inject();
    } catch(const std::exception &error) {
        std::cerr << error.what();
        return EXIT_FAILURE;
    }

    std::cout << "Successfully injected DLL\n";

    return EXIT_SUCCESS;
}
