#pragma once
#include "dllexport.h"
#include <cstddef>

namespace Core {
	class DLLEXPORT IComponent {
	public:
		virtual std::size_t getTypeID() = 0;
	};
}