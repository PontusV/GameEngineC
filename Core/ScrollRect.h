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
		void onPostLayoutUpdate() override;
		bool hasChanged();

		void onChildAdded(EntityHandle entity) override;
		void onChildRemoved(EntityHandle entity) override;

		const Bounds& getContentBounds() const;

	private:
		void refreshContentBounds();
		void checkScrollArea();

	public:
		Vector2 offset;
		PROPERTY()
		float speed = 30.0f;
		PROPERTY()
		bool scrollableY = true;
		PROPERTY()
		bool scrollableX = false;
		PROPERTY()
		float paddingTop = 0;
		PROPERTY()
		float paddingRight = 0;
		PROPERTY()
		float paddingBottom = 0;
		PROPERTY()
		float paddingLeft = 0;


	private:
		bool changed = false;
		Bounds contentBounds;
	};
}
#endif