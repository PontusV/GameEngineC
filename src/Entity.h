#ifndef ENTITY_H
#define ENTITY_H
#include "Serializable.h"
#include <vector>
#include "Handle.h"

/* An object with an ID. An entity may be linked with one or more components. */
class Entity : public Serializable {
public:
	Entity();
	~Entity();

	void setID(Handle handleID);
	Handle getID() const;

	//Save & Load operator
	void serialize(std::ostream& os) const;
	void deserialize(std::istream& is);

	bool operator <(const Entity& rhs) const
	{
		return id < rhs.getID();
	}
private:
	Handle id;
};
#endif