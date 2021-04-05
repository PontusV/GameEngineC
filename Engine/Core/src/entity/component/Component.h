#ifndef COMPONENT_H
#define COMPONENT_H
#include <ReflectionParser/ReflectionMacros.h>
#include "entity/handle/Handle.h"
#include "utils/UIDContainer.h"
#include "utils/Serializable.h"
#include "Component.generated.h"

typedef std::size_t ComponentTypeID; // Should not be needed here. Remove?
typedef std::size_t ComponentID;

namespace Core {
	CLASS() Component : public Serializable {
		GENERATED_BODY()
	protected:
		Component();
	public:
		Component(const Component& other);
		Component& operator=(const Component& other);
		virtual ~Component() = 0; // Abstract

		Handle getOwner();
		void setOwner(Handle owner);

		ComponentID getComponentID() const;

	protected:
		Handle owner;

	private:
		ComponentID componentID;
		static UIDContainer<1000000> uniqueIDContainer;
	};
}

#endif