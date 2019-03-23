#ifndef COMPONENT_H
#define COMPONENT_H
#include "Serializable.h"
#include "Entity.h"
#include "ChunkEntityHandle.h"

namespace Core {

	typedef std::size_t ComponentTypeID;

	class IChunk; // Forward declare

	class Component : public Serializable {
	protected:
		Component();
	public:
		virtual ~Component();

		virtual const ComponentTypeID getTypeID() const = 0;
		Component& operator=(const Component& other)
		{
			owner = other.owner;
			return *this;
		}

		void setOwner(ChunkEntityHandle owner)	{ this->owner = owner; }

		void destroy() { destruct = true; }
		bool isDestroyed() { return destruct; }

		// The components that need this will override
		virtual void init() {}; // Called after setOwner()
		virtual void end() {}; // Called before component values are copied in a move
	protected:
		// All the data needed to get the components from the same entity
		ChunkEntityHandle owner;
	private:
		bool destruct = false;
		//ComponentHandle handle;
		//Handle handle;
	};
}

//std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif