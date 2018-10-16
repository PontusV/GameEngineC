#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include "Entity.h"

#include "ComponentDataManager.h"
#include "Image.h"
#include "Text.h"
#include "Transform.h"

class HandleManager;
class Handle;
class Window;

class Graphics {
public:
	Graphics(HandleManager& handleManager);
	~Graphics();

	void initiate();
	void update(float dt);
	void createWindow();

	//Handle createImage(Entity entity, std::string fileName);
	//Handle createText(Entity entity, const std::string& text, std::string fontAddress, int fontSize, SDL_Color color);

	//Save & Load
	void write(std::ostream& os) const;
	void read(std::istream& is, HandleManager* handleManager);
private:
	ComponentDataManager<1024, Image, Transform> renderables;
	//ComponentDataManager<Text, Transform> renderableTexts;

	//ComponentData<Image, Transform> data = new ComponentData<Image, Transform>(maxSize);

	Window* window;
	HandleManager& handleManager; //Initialize this
};
#endif