#ifndef SCROLL_RECT_H
#define SCROLL_RECT_H
#include "UIBehaviour.h"
#include "Maths/Vector2.h"
#include "Bounds.h"
#include "ScrollRect.generated.h"
namespace Core {
	CLASS() ScrollRect : public UIBehaviour {
		GENERATED_BODY()
	public:
		ScrollRect();
		~ScrollRect();

		void onScroll(float xOffset, float yOffset) override;
		void lateUpdate(float deltaTime) override;
		bool hasChanged();

	private:
		void refreshContentBounds();

	public:
		Vector2 offset;
		PROPERTY()
		float speed = 30.0f;
		PROPERTY()
		bool scrollableY = true;

	private:
		bool changed = false;
		Bounds contentBounds;
	};
}
#endif