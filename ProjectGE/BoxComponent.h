#ifndef BOX_COMPONENT_H
#define BOX_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace Core {
	/* A type of component with a size. Calculates offset from Transform component, depending on Transform.anchor
	 * Abstract class. Requires a Transform component in the same Entity to function. */
	class BoxComponent : public Component {
		REGISTER_COMPONENT_TYPE(10);
	public:
		virtual ~BoxComponent();

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
		glm::vec2	offset;			// Offset from Transform.position. Defines where the TOP_LEFT vertex of this box is located.
	};
}
#endif