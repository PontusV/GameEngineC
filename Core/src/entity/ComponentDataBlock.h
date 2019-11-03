#ifndef COMPONENT_DATA_BLOCK_H
#define COMPONENT_DATA_BLOCK_H
#include <cstddef>
namespace Core {
	struct ComponentDataBlock {
		void* ptr;
		std::size_t typeID;
		std::size_t size;
	};
}
#endif