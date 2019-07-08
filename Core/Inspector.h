#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Behaviour.h"
#include <ReflectionParser/ReflectionTypes.h>
#include "Inspector.Inspector.generated.h"

namespace Core {

	struct PropertyInstance {
		Component* instance;
		Mirror::Property prop;
	};

	CLASS() Inspector : public Behaviour {
		GENERATED_BODY()
	public:
		Inspector();
		~Inspector();

		void awake() override;
		void onMouseButtonPressed(int buttoncode, int mods) override;

		void inspect(EntityHandle entity);
		void onPropertyValueSubmit(std::wstring value);
		void onPropertyValueToggle(bool value);
		void onBoolPropertyValueSubmit(bool value);

	private:
		void clearEntries();
		void addComponentEntry(Component* component, std::size_t id);

		EntityHandle createPropertyField(std::string fieldName, Mirror::Property& prop, Component* component);
		std::wstring propertyValueToString(Mirror::Property& prop, Component* component) const;
		bool getSelectedProperty(PropertyInstance& ref);

	private:
		EntityHandle currentTarget;
		EntityHandle scrollPanel;
		std::vector<EntityHandle> targetComponentList;
	};
}
#endif