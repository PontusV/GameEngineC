#ifndef UI_OBJECT_DATA_H
#define UI_OBJECT_DATA_H
#include "ObjectData.h"
#include "UIObjectData.generated.h"
namespace Core {
	CLASS(readonly, hideInInspector) UIObjectData : public ObjectData {
		GENERATED_BODY()
	public:
		UIObjectData(Scene* scene) : ObjectData(ObjectType::UI, scene) {}
		~UIObjectData() {}
	};
}
#endif