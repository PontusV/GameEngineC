#ifndef PROPERTY_FIELD_H
#define PROPERTY_FIELD_H
#include "Behaviour.h"
#include <string>
#include "PropertyField.generated.h"

namespace Core {
	CLASS() PropertyField : public Behaviour {
		GENERATED_BODY()
	public:
		PropertyField(std::string fieldName, Mirror::Property& prop, Component* component);
		~PropertyField();

		void awake() override;

	private:

	};
}
#endif