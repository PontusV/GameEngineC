#ifndef CANVAS_RENDERER_H
#define CANVAS_RENDERER_H

#include "Renderer.h"
#include "CanvasRenderer.generated.h"

namespace Core {

	/* UI Graphic. */
	CLASS() CanvasRenderer : public Renderer {
		GENERATED_BODY()
	public:
		CanvasRenderer();
		~CanvasRenderer();
	private:

	};
}
#endif