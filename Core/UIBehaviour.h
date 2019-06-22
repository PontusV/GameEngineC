#ifndef UI_BEHAVIOUR_H
#define UI_BEHAVIOUR_H
#include "Behaviour.h"
#include "UIBehaviour.UIBehaviour.generated.h"
namespace Core {
	/* A behaviour with additional onEvent functions related to UI. */
	CLASS() UIBehaviour : public Behaviour {
		GENERATED_BODY()
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