#ifndef COMPONENT_H
#define COMPONENT_H
#include "Serializable.h"
#include "Handle.h"

typedef std::size_t ComponentTypeID; // Should not be needed here. Remove

namespace Core {
	class Component : public Serializable {
	protected:
		Component();
	public:
		virtual ~Component() = 0; // Abstract

		Handle&							getOwner();
		void							setOwner(Handle owner);
		/* Marks the Component for destruction. */
		void							destroy();
		/* Looks if the Component is marked for destruction. */
		bool							isDestroyed();

		// Serializable
		virtual void					serialize(std::ostream& os) const;
		virtual void					deserialize(std::istream& is);

	protected:
		Handle owner;

	private:
		bool destruct = false;
	};
}

//std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif