#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H
#include "Behaviour.h"
#include "PropertyValueID.h"
#include "ReflectedObjectHandle.h"
#include "ComponentHandle.h"
#include <string>
#include "PropertyEditor.generated.h"
namespace Core {

	enum class PropertyFieldType {
		INVALID=0,
		INPUT_FIELD,
		CHECKBOX
	};

	CLASS() PropertyEditor : public Behaviour {
		GENERATED_BODY()
	public:
		PropertyEditor(PropertyValueID value, Mirror::Property& rootProp, ReflectedObjectHandle instance) : valueID(value), rootProp(rootProp), instanceHandle(instance) {}
		~PropertyEditor() {}

		void start() override;
		void update(float deltaTime) override;

		void onTextSubmit(std::wstring value);
		void onBoolSubmit(bool value);
		static std::wstring propertyValueToString(PropertyValueID valueID, ReflectedObjectHandle instanceHandle);

	private:
		PropertyValueID valueID;
		ReflectedObjectHandle instanceHandle;
		Mirror::Property rootProp; // The property to call onUpdate for when value is set

		ComponentHandle display;
		PropertyFieldType displayType = PropertyFieldType::INVALID;
		static const float PROPERTY_UPDATE_DELAY;
		float currentPropertyUpdateTime = 0.0f;
	};
}
#endif