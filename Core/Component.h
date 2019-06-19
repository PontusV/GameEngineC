#ifndef COMPONENT_H
#define COMPONENT_H
#include "ReflectionMacros.h"
#include "Serializable.h"
#include "EntityHandle.h"
#include "Component.Component.generated.h"

typedef std::size_t ComponentTypeID; // Should not be needed here. Remove

namespace Core {
	CLASS() Component : public Serializable {
		GENERATED_BODY()
	protected:
		Component();
	public:
		virtual ~Component() = 0; // Abstract

		EntityHandle& getOwner();
		void setOwner(Handle owner);
		/* Marks the Component for destruction. */
		void destroy();
		/* Looks if the Component is marked for destruction. */
		bool isDestroyed();

	protected:
		EntityHandle owner;

	private:
		bool destruct = false;
	};
}

//std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif