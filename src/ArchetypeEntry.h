#ifndef ARCHETYPE_ENTRY_H
#define ARCHETYPE_ENTRY_H
#include <vector>
#include <utility>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include <initializer_list>
#include "Archetype.h"
#include "ComponentType.h"

namespace GameEngine {
	class ArchetypeEntry {
	public:
		ArchetypeEntry(std::shared_ptr<IArchetype> archetype, const std::initializer_list<ComponentTypeID> types) : archetype(archetype), types(types) {}
		ArchetypeEntry(std::shared_ptr<IArchetype> archetype, std::vector<ComponentTypeID> types) : archetype(archetype), types(types) {}
		~ArchetypeEntry() {
			types.clear();
		}
		/* Looks if the given list of types are contained in this Archetype. Depends on the types list to be sorted for efficient lookup of types. Given vector of types must be sorted. */
		bool contains(std::vector<ComponentTypeID>& otherTypes) const {
			std::size_t i = 0;
			for (ComponentTypeID& otherType : otherTypes) {
				//Looks if otherType exist in types
				while (i < types.size()) {
					if (types[i] < otherType) { //Only works if types list is sorted
						return false;
					}
					else if (types[i] == otherType) {
						i++;
						break;
					}
					else if (i == types.size()) {
						return false;
					}
					i++;
				}
			}
			return true;
		}

		/* Looks if given list of types contains the same types as list contained by this Archetype. Given vector of types must be sorted. */
		bool match(std::vector<ComponentTypeID>& otherTypes) const {
			if (otherTypes.size() != types.size())
				return false;

			for (std::size_t i = 0; i < types.size(); i++) {
				if (types[i] != otherTypes[i])
					return false;
			}
			return true;
		}

		std::shared_ptr<IArchetype> getArchetype() const {
			return archetype;
		}

		const std::vector<ComponentTypeID>& getTypes() const {
			return types;
		}

	private:
		std::vector<ComponentTypeID> types;
		std::shared_ptr<IArchetype> archetype;
	};
}
#endif