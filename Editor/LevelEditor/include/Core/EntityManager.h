#pragma once
#include "dllexport.h"

namespace Core {
	class DLLEXPORT IEntityManager {
	public:
		virtual bool isEntityNameAvailable(const char* name) = 0;
	};
}