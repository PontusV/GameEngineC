#ifndef GAME_OBJECT_DATA_H
#define GAME_OBJECT_DATA_H
#include "ObjectData.h"
#include "GameObjectData.generated.h"
namespace Core {
	CLASS(readonly, hideInInspector) GameObjectData : public ObjectData {
		GENERATED_BODY()
	public:
		GameObjectData(Scene* scene) : ObjectData(ObjectType::World, scene) {}
		~GameObjectData() {}
	};
}
#endif