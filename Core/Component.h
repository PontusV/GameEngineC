#ifndef COMPONENT_H
#define COMPONENT_H
#include "ReflectionMacros.h"
#include "Serializable.h"
#include "EntityHandle.h"
#include "UIDContainer.h"
#include "Component.Component.generated.h"

typedef std::size_t ComponentTypeID; // Should not be needed here. Remove
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

		EntityHandle getOwner() const;
		void setOwner(Handle owner);
		/* Marks the Component for destruction. */
		void destroy();
		/* Looks if the Component is marked for destruction. */
		bool isDestroyed();

		ComponentID getComponentID() const;

	protected:
		EntityHandle owner;

	private:
		bool destruct = false;
		ComponentID componentID;
		static UIDContainer<1000000> uniqueIDContainer;
	};
}

#endif