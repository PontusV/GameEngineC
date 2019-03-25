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

		virtual const ComponentTypeID	getTypeID() const = 0;

		void			setOwner(ChunkEntityHandle owner);
		void			destroy();
		bool			isDestroyed();

		// The components that need this will override
		virtual void	init() {};	// Called after setOwner()
		virtual void	end() {};	// Called before component values are copied in a move

		// Serializable
		virtual void	serialize(std::ostream& os) const;
		virtual void	deserialize(std::istream& is);

	protected:
		ChunkEntityHandle owner;

	private:
		bool destruct = false;
	};
}

//std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif