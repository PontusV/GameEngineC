#include "GUID.h"
#include <stdexcept>
#ifdef WIN32
#pragma comment(lib, "rpcrt4.lib")
#include <Windows.h>
#endif
using namespace Core;

std::string Core::GUID::create() {
    std::string result;
#ifdef WIN32
    UUID uuid;
    if (UuidCreate(&uuid) != RPC_S_OK) {
        throw "Core::GUID::create::ERROR Failed to create GUID";
    }
    char* str;
    if (UuidToStringA(&uuid, (RPC_CSTR*)&str) != RPC_S_OK) {
        throw "Core::GUID::create::ERROR Failed to convert GUID to string";
    }
    result = str;
    RpcStringFreeA((RPC_CSTR*)&str);
    return result;
#endif
    throw "Core::GUID::create::ERROR Invalid platform";
}