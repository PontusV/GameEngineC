#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include "Entity.h"

#include "Image.h"
#include "Text.h"
#include "Transform.h"

#include "ComponentArrayManager.h"
#include "ComponentGroup.h"

namespace GameEngine {
	class HandleManager;
	class Handle;
	class Window;

	struct Renderables : public ComponentGroup<Transform, Image> {
		ComponentArray<Transform>& transforms = getComponentArray<Transform>();
		ComponentArray<Image>& images = getComponentArray<Image>();
	};

	class Graphics {
	public:
		Graphics(HandleManager& handleManager);
		~Graphics();

		void initiate();
		void update(float dt);
		void createWindow();
	private:

		Window* window;
		HandleManager& handleManager; //Initialize this
		Renderables renderables;
	};
}
#endif