#ifndef GRAPHIC_COMPONENT_H
#define GRAPHIC_COMPONENT_H
#include "BoxComponent.h"

#include <glm/glm.hpp>

namespace Core {
	/* Enables clipping; removes graphic drawn outside the given rect. */
	class GraphicComponent : public BoxComponent {
	public:
		virtual ~GraphicComponent();
	public:
		void				setLayerIndex(unsigned short index);
		unsigned short		getLayerIndex();

		const glm::vec4&	getDrawRect();
		void				setDrawRect(glm::vec4 rect);

		void				enableClipping();
		void				disableClipping();
		bool				isClipEnabled();
	protected:
		GraphicComponent(unsigned short layerIndex = 0, unsigned int width = 0, unsigned int height = 0);
	private:
		unsigned short	layerIndex;
		bool			clipEnabled;
		glm::vec4		drawRect;
	};
}
#endif