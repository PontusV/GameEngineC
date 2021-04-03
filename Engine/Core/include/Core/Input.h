#pragma once
#include "dllexport.h"
#include "EntityHandle.h"

namespace Core {
	class DLLEXPORT IInput {
	public:
		/* Creates a new Handle and allocated in heap memory. Note: has to be explicitly released to free memory */
		virtual IEntityHandle* createEntityHandleAtPos(float x, float y) = 0;
	};
}