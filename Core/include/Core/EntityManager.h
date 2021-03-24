#pragma once
#include "dllexport.h"

namespace Core {
	extern "C" class DLLEXPORT IEntityManager {
	public:
		virtual bool isEntityNameAvailable(const char* name) = 0;
	};
}