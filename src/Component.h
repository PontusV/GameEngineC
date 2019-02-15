#ifndef COMPONENT_H
#define COMPONENT_H
#include "Serializable.h"
#include "Handle.h"

namespace GameEngine {
	class Entity;
	class Engine;

	typedef std::size_t ComponentTypeID;

	class Component : public Serializable {
	public:
		virtual ~Component();
		void setID(Handle handleID);

		virtual const ComponentTypeID getTypeID() const = 0;
		Handle getID() const;
		Component& operator=(const Component& rhs)
		{
			//Do not change ID
			return *this;
		}
	protected:
		Component();
	private:
		Handle componentID;
	};
}

//std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif