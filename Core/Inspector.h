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

	private:
		EntityHandle currentTarget;
	};
}
#endif