#ifndef ENTITY_LOCATION_H
#define ENTITY_LOCATION_H

#include <memory>

namespace Core {

	class Chunk; // Forward declare

	/* Stores the location of an Entity in memory. Contains a pointer to its chunk and the index in chunk. */
	struct EntityLocation {
		EntityLocation(std::size_t index, std::weak_ptr<Chunk> chunk) : index(index), chunk(chunk) {}
		EntityLocation() : index(0), chunk(std::weak_ptr<Chunk>()) {}
		std::size_t index;
		std::weak_ptr<Chunk> chunk = std::weak_ptr<Chunk>();
	};
}
#endif