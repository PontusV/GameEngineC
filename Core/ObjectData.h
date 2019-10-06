#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H
#include "Component.h"
#include "ObjectData.generated.h"
namespace Core {
	enum class ObjectType {
		World=0,
		UI
	};

	class Scene;

	/* A componented owned by every World and UI object. */
	CLASS(readonly, hideInInspector) ObjectData : public Component {
		GENERATED_BODY()
	public:
		~ObjectData() {}
		ObjectType getLayer() { return type; }
		Scene* getScene() { return scene; }
	protected:
		ObjectData(ObjectType type, Scene* scene) : type(type), scene(scene) {}
	private:
		Scene* scene;
		const ObjectType type;
		// tag
		// name
	};
}
#endif