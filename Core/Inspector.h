#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Behaviour.h"
#include <ReflectionParser/ReflectionTypes.h>
#include "PropertyValueID.h"
#include "ReflectedObjectHandle.h"
#include "Inspector.generated.h"

namespace Core {

	class ReflectedObject;

	struct PropertyInstance {
		std::size_t lineIndex;
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

	private:
		void clearEntries();
		void addComponentEntry(Component* component, std::size_t id);
		EntityHandle createPropertyField(std::string name, Mirror::Property& prop, Component* component, ReflectedObject* instance);
		EntityHandle createPropertyValueField(std::string label, PropertyValueID value, Component* root, ReflectedObject* instance, std::string entityName);

	private:
		EntityHandle currentTarget;
		EntityHandle scrollPanel;
		std::vector<EntityHandle> targetComponentList;
	};
}
#endif