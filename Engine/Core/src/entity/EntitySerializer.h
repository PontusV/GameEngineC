#ifndef ENTITY_SERIALIZER_H
#define ENTITY_SERIALIZER_H

#include "entity/component/ComponentLoader.h"
#include "entity/handle/Handle.h"
#include "entity/handle/EntityHandle.h"
#include <iostream>
#include "ReflectionPolymorph.generated.h"

namespace Core {
	template<typename Archive>
	static void serializeEntity(Handle handle, Archive& archive) {
		std::vector<IComponentData*> components = handle.getComponents();
		std::vector<IComponentTypeInfo> componentTypeInfoVec = handle.getComponentTypes();
		std::size_t componentAmount = components.size();
		archive(componentAmount);									// Component amount

		for (std::size_t i = 0; i < componentAmount; i++) {			// Loop through Components
			std::string typeName = componentTypeInfoVec[i].name;	// TODO: Use SerializationID instead
			std::size_t typeID = componentTypeInfoVec[i].type.getTypeID();
			archive(typeName);										// Component Type name
			Mirror::serialize(components[i], typeID, archive);		// Component Data
		}
	}

	template<typename Archive>
	static void deserializeEntity(EntityHandle handle, Archive& archive) {
		std::size_t componentAmount;
		archive(componentAmount);												// Component amount

		for (std::size_t i = 0; i < componentAmount; i++) {						// Loop through Components
			std::string typeName;												// TODO: Use SerializationID instead
			archive(typeName);													// Component Type name
			ComponentLoader::addComponentFromFile(archive, typeName, handle);	// Component Data
		}
	}
}
#endif