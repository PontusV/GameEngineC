#ifndef BOX_COMPONENT_H
#define BOX_COMPONENT_H

#include "Component.h"
#include "Bounds.h"
#include <glm/glm.hpp>

namespace Core {
	/* A type of component with a size.
	 * Abstract class. Requires a Transform component attached to the same Entity.
	 */
	class BoxComponent : public Component {
	public:
		virtual ~BoxComponent() = 0; // Abstract

		/* Recalculates bounds. */
		void				updateBounds();
		Bounds&				getBounds();
		const glm::ivec2&	getSize() const;
		void				setSize(unsigned int width, unsigned int height);
		void				setSize(glm::ivec2 size);

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	protected:
		BoxComponent(unsigned int width, unsigned int height);
		BoxComponent(glm::ivec2 size);
		BoxComponent();

	private:
		glm::ivec2	size;
		Bounds		bounds;
	};
}
#endif