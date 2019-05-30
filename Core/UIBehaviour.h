#ifndef UI_BEHAVIOUR_H
#define UI_BEHAVIOUR_H
#include "Behaviour.h"
namespace Core {
	/* A behaviour with additional onEvent functions related to UI. */
	class UIBehaviour : public Behaviour {
	public:
		virtual ~UIBehaviour() = 0;
		
		/* Called when an attached RectTransform has changed size. */
		virtual void onRectTransformResized() {}
		/* Called when an immediate child has an attached RectTransform change size. */
		virtual void onChildRectTransformResized() {}
		/* Called when the window changes resolution. */
		virtual void onWindowResize() {}

	protected:
		UIBehaviour();
	};
}
#endif