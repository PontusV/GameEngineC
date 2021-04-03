#ifndef ENGINE_DLL_H
#define ENGINE_DLL_H

#include <Core/Core.h>
#include <Windows.h>
#include <memory>

typedef Core::DLLInterface* (*InitFuncType)();
typedef void (*ReleaseFuncType)(Core::DLLInterface*);

namespace Editor {
	class EngineDLL {
	public:
		EngineDLL();
		~EngineDLL();

		/* Loads DLL at specified path. Expects path to be pointing towards an Engine DLL. Returns true if DLL has been loaded successfully */
		bool load(const wchar_t* path);
		/* Unloads the DLL. Releases allocated memory for the DLLInterface. Returns true if DLL has been unloaded successfully */
		bool unload();
		/* Returns true if DLL has been loaded successfully */
		bool isLoaded() const;

		Core::DLLInterface* getInterface() const;
	private:
		HINSTANCE handle;
		ReleaseFuncType releaseInterfaceFunc;
		Core::DLLInterface* interfacePtr;
	};
}
#endif