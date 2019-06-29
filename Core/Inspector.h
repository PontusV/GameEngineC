#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Behaviour.h"
#include "Inspector.Inspector.generated.h"

namespace Core {
	CLASS() Inspector : public Behaviour {
		GENERATED_BODY()
	public:
		Inspector();
		~Inspector();

		void awake() override;
		void onMouseButtonPressed(int buttoncode, int mods) override;

		void inspect(EntityHandle entity);
		void onPropertyValueSubmit(std::wstring value);

	private:
		void clearEntries();
		void addComponentEntry(Component* component);

		EntityHandle createPropertyField(std::string fieldName, Mirror::Property& prop, Component* component);
		std::string propertyValueToString(Mirror::Property& prop, Component* component) const;

	private:
		EntityHandle currentTarget;
		EntityHandle scrollPanel;
		std::vector<EntityHandle> targetComponentList;

		std::size_t entryCount = 0;
	};
}
#endif