#ifndef ARCHETYPE_ENTRY_H
#define ARCHETYPE_ENTRY_H
#include "Archetype.h"
#include "ComponentType.h"
#include <vector>
#include <memory>
#include <initializer_list>

namespace GameEngine {
	class ArchetypeEntry {
	public:
		ArchetypeEntry(std::shared_ptr<IArchetype> archetype, const std::initializer_list<ComponentTypeID> types);
		ArchetypeEntry(std::shared_ptr<IArchetype> archetype, std::vector<ComponentTypeID> types);
		~ArchetypeEntry();
		/* Looks if the given list of types are contained in this Archetype. Depends on the types list to be sorted for efficient lookup of types. Given vector of types must be sorted. */
		bool contains(std::vector<ComponentTypeID>& otherTypes) const;

		/* Looks if given list of types contains the same types as list contained by this Archetype. Given vector of types must be sorted. */
		bool match(std::vector<ComponentTypeID>& otherTypes) const;

		std::shared_ptr<IArchetype> getArchetype() const;

		const std::vector<ComponentTypeID>& getTypes() const;

	private:
		std::vector<ComponentTypeID> types;
		std::shared_ptr<IArchetype> archetype;
	};
}
#endif