#ifndef LEVEL_H
#define LEVEL_H
#include <map>
#include <vector>
#include "Serializable.h"
#include "Handle.h"
#include <string>
#include "Transform.h"

class Entity;
class EntityHandle;
class HandleManager;

/* Consists of multiple entities.
*/
class Level : public Serializable {
public:
	Level(HandleManager& handleManager);
	~Level();
	EntityHandle createEntity(std::string name); //Creates an entity and maps the given name to it
	void removeEntity(Handle id);
	EntityHandle getEntity(std::string entityName);
	//Save & Load operator
	void serialize(std::ostream& os) const;
	void deserialize(std::istream& is);
private:
	std::map<std::string, Entity*> entities; //Name of Entity, Entity ptr
	HandleManager& handleManager;
	//List of ComponentManagers for all ComponentTypes used in this level/scene/world
};
#endif