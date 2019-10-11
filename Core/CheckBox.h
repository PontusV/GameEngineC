#ifndef CHECK_BOX_H
#define CHECK_BOX_H
#include "Behaviour.h"
#include "ComponentFunctionHandle.h"
#include "CheckBox.generated.h"
namespace Core {
	CLASS() CheckBox : public Behaviour {
		GENERATED_BODY()
	public:
		CheckBox(bool toggle = false);
		~CheckBox();
		void start() override;
		void onDestroy() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;

		void setToggle(bool value);
		bool getToggle() const;

		PROPERTY()
		ComponentFunctionHandle<void, bool> onToggle;
	private:
		void showTick();
		void hideTick();

	private:
		PROPERTY()
		bool toggle = false;
		
		EntityHandle tick;
		EntityHandle background;
		EntityHandle border;
	};
}
#endif