#pragma once
#include "dllexport.h"
#include "Window.h"
#include "Camera.h"

namespace Core {
	extern "C" class DLLEXPORT IGraphics {
	public:
		virtual IWindow* getWindowInterface() = 0;
		virtual ICamera* getCameraInterface() = 0;
	};
}