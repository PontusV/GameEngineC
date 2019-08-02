#ifndef BOX_COMPONENT_H
#define BOX_COMPONENT_H

#include "Component.h"
#include "Bounds.h"
#include <glm/glm.hpp>
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
		const glm::ivec2& getSize() const;

		void setSize(int width, int height);
		void setSize(glm::ivec2 size);
		void setWidth(int width);
		void setHeight(int height);

	protected:
		BoxComponent(int width, int height);
		BoxComponent(glm::ivec2 size);
		BoxComponent();

	private:
		PROPERTY()
		glm::ivec2 size;
		PROPERTY()
		Bounds bounds;
	};
}
#endif