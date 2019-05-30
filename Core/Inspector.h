#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Behaviour.h"
namespace Core {
	class Inspector : public Behaviour {
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