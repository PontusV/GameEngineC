#ifndef SELECTABLE_H
#define SELECTABLE_H

#include "Behaviour.h"
#include "Selectable.generated.h"

namespace Core {
	CLASS() Selectable : public Behaviour {
		GENERATED_BODY()
	public:
		virtual ~Selectable();

		void select();
		void deselect();
		virtual void onSelect() {}
		virtual void onDeselect() {}
		bool isSelected();

	protected:
		Selectable();

	private:
		bool selected = false;
	};
}
#endif