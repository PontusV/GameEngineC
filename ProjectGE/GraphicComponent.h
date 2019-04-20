#ifndef GRAPHIC_COMPONENT_H
#define GRAPHIC_COMPONENT_H
#include "BoxComponent.h"

#include <glm/glm.hpp>

namespace Core {
	/* Enables clipping; removes graphic drawn outside the given rect. */
	class GraphicComponent : public BoxComponent {
	public:
		virtual ~GraphicComponent() = 0; // Abstract

		void					setLayerIndex(unsigned char index);
		const unsigned char&	getLayerIndex() const;

		const std::vector<glm::vec2>& getClipMaskVertices() const;

		/* Cuts away Drawable area that is outside of the rect parameter. */
		void					clip(glm::vec2 clipVertices[4]);
		void					resetClipping();
		bool					isClipEnabled() const;
		
		// Serializable
		virtual void			serialize(std::ostream& os) const;
		virtual void			deserialize(std::istream& is);

	protected:
		GraphicComponent(unsigned char layerIndex = 0, unsigned int width = 0, unsigned int height = 0);

	private:
		unsigned char			layerIndex;
		bool					clipEnabled;
		std::vector<glm::vec2>	clipMaskVertices;
		//glm::vec4		drawRect;
	};
}
#endif