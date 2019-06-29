#ifndef SELECTABLE_H
#define SELECTABLE_H

#include "Behaviour.h"
#include "Selectable.Selectable.generated.h"

namespace Core {
	CLASS() Selectable : public Behaviour {
		GENERATED_BODY()
	public:
		Selectable();
		virtual ~Selectable();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonPressed(int buttoncode, int mods) override;
		virtual void onSelect() {}
		virtual void onDeselect() {}
		bool isSelected();

	private:
		bool selected = false;
	};
}
#endif