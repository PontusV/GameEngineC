#ifndef ENGINE_DLL_H
#define ENGINE_DLL_H

#include <Windows.h>
#include <cstddef>
#include <string>
#include "imgui/imgui.h"
#include "EngineDLLInterface.h"

namespace Editor {
	class EngineDLL : public EngineDLLInterface {
	public:
		EngineDLL();
		~EngineDLL();

		/* Loads DLL at specified path. Expects path to be pointing towards an Engine DLL. Returns true if DLL has been loaded successfully */
		bool load(const wchar_t* path);
		/*
			MAKE SURE ALL ALLOCATED RESOURCES HAVE BEEN RELEASED BEFORE USE.
			Unloads the DLL. Releases allocated memory for the DLLInterface. Returns true if DLL has been unloaded successfully
		*/
		bool unload();
		/* Returns true if DLL has been loaded successfully */
		bool isLoaded() const;
		bool isAltActivated() const;
		std::wstring getNextDLLName();
		std::wstring getCurrentDLLName();

		std::size_t getEntitySceneIndex(EntityID entityID);
	private:
		HINSTANCE handle;
		bool altActivated = true; // Engine DLL loaded from alternative path
	};
}
#endif