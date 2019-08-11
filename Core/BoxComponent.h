#ifndef BOX_COMPONENT_H
#define BOX_COMPONENT_H

#include "Component.h"
#include "Bounds.h"
#include "Maths/Vector2.h"
#include "BoxComponent.generated.h"

namespace Core {
	/* A type of component with a size.
	 * Abstract class. Requires a Transform component attached to the same Entity.
	 */
	CLASS() BoxComponent : public Component {
		GENERATED_BODY()
	public:
		virtual ~BoxComponent() = 0; // Abstract

		/* Recalculates bounds. */
		void updateBounds();
		Bounds& getBounds();
		const Vector2& getSize() const;

		void setSize(int width, int height);
		void setSize(Vector2 size);
		void setWidth(int width);
		void setHeight(int height);

	protected:
		BoxComponent(int width, int height);
		BoxComponent(Vector2 size);
		BoxComponent();

	private:
		PROPERTY()
		Vector2 size;
		PROPERTY()
		Bounds bounds;
	};
}
#endif