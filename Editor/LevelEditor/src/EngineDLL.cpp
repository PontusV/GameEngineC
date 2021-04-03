#include "EngineDLL.h"
#include <iostream>
using namespace Editor;


EngineDLL::EngineDLL() : handle(NULL) {}
EngineDLL::~EngineDLL() {}

bool EngineDLL::load(const wchar_t* path) {
    if (handle != NULL) {
        std::cout << "Unable to load DLL because one is currently already loaded" << std::endl;
        return false;
    }
    handle = LoadLibrary(path);
    if (handle == NULL) {
        std::cout << "Failed to create handle for DLL at specified path" << std::endl;
        return false;
    }

    InitFuncType init = (InitFuncType)(GetProcAddress(handle, "createDLLInterface"));
    releaseInterfaceFunc = (ReleaseFuncType)(GetProcAddress(handle, "releaseDLLInterface"));

    if (!releaseInterfaceFunc) {
        std::cout << "Failed to get address of releaseDLLInterface function from DLL at specified path" << std::endl;
        unload();
        return false;
    }
    if (!init) {
        std::cout << "Failed to get address of createDLLInterface function from DLL at specified path" << std::endl;
        unload();
        return false;
    }

    interfacePtr = init();
    return true;
}

bool EngineDLL::unload() {
    if (handle == NULL) {
        std::cout << "Unable to unload DLL because none is currently loaded" << std::endl;
        return false;
    }
    if (releaseInterfaceFunc && interfacePtr) {
        releaseInterfaceFunc(interfacePtr);
    }
    releaseInterfaceFunc = nullptr;
    interfacePtr = nullptr;
    FreeLibrary(handle);
    return true;
}

bool EngineDLL::isLoaded() const {
    return handle != NULL;
}

Core::DLLInterface* EngineDLL::getInterface() const {
    if (isLoaded()) {
        return interfacePtr;
    }
    return nullptr;
}