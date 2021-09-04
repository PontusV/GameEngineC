#ifndef ENTITY_LOCATION_H
#define ENTITY_LOCATION_H

#include <memory>

namespace Core {

	class Archetype; // Forward declare
	class Chunk; // Forward declare

	/* Stores the location of an Entity in memory. Contains a pointer to its chunk and the index in chunk. */
	struct EntityLocation {
		EntityLocation(std::size_t index, std::weak_ptr<Chunk> chunk) : index(index), chunk(chunk) {}
		EntityLocation() : index(0), chunk(std::weak_ptr<Chunk>()) {}
		std::size_t index;
		std::weak_ptr<Chunk> chunk = std::weak_ptr<Chunk>();
		bool isValid() const { return chunk.lock() != nullptr; }
	};

	/* Contains the location of the Entity in a chunk as well as its archetype */
	struct EntityLocationDetailed : public EntityLocation {
		EntityLocationDetailed() {}
		EntityLocationDetailed(std::size_t index, std::weak_ptr<Chunk> chunk, std::weak_ptr<Archetype> archetype) : archetype(archetype), EntityLocation(index, chunk) {}
		std::weak_ptr<Archetype> archetype;
		bool isValid() const { return chunk.lock() != nullptr && archetype.lock() != nullptr; }
	};
}
#endif