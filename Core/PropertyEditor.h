#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H
#include "Component.h"
#include "PropertyValueID.h"
#include "ReflectedObjectHandle.h"
#include <string>
#include "PropertyEditor.generated.h"
namespace Core {
	CLASS() PropertyEditor : public Component {
		GENERATED_BODY()
	public:
		PropertyEditor(PropertyValueID value, ReflectedObjectHandle instance) : valueID(value), instanceHandle(instance) {}
		~PropertyEditor() {}

		void onTextSubmit(std::wstring value);
		void onBoolSubmit(bool value);
		static std::wstring propertyValueToString(PropertyValueID valueID, ReflectedObjectHandle instanceHandle);

	private:
		PropertyValueID valueID;
		ReflectedObjectHandle instanceHandle;
	};
}
#endif