#ifndef PANEL_H
#define PANEL_H
#include "BoxComponent.h"
#include "Panel.Panel.generated.h"

namespace Core {

	/* Clips all GraphicComponents contained by all children; panels remove graphics drawn by its children that resides outside of the panel. */
	CLASS() Panel : public BoxComponent {
		GENERATED_BODY()
	public:
		Panel(unsigned int width, unsigned int height);
		Panel();
		~Panel();
	};
}
#endif