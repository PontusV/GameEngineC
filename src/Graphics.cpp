#include "Graphics.h"
#include "Window.h"
#include "Image.h"
#include "Text.h"
#include "HandleManager.h"
#include "Handle.h"


Graphics::Graphics(HandleManager& handleManager) : handleManager(handleManager), renderables(handleManager) {}


Graphics::~Graphics() {
	delete window;
}

void Graphics::initiate() {
	
}

/* Loops through Renderable vector and draws them.*/
void Graphics::update(float dt) {
	renderables.update();
	auto& renderData = renderables.getAll();
	for (int i = 0; i < renderData.size(); i++) {
		auto& renderable = renderData[i];
		Image& image = std::get<0>(renderable);
		Transform& transform = std::get<1>(renderable);

		image.reload(window->getRenderer()); //Make sure image is loaded

		//Get draw variables
		int x = int(transform.getX());
		int y = int(transform.getY());
		int width = image.getTexture()->getWidth();
		int height = image.getTexture()->getHeight();

		//Draw
		SDL_Rect dest{ x, y, width, height };
		SDL_RenderCopy(window->getRenderer(), image.getTexture()->getTexture(), NULL, &dest);
	}
	window->update();
	//int x, y, width, height;
	//Enable code after renderables have been intialized
	/*ComponentData<Image, Transform>& renderData = renderables.getAll();
	for (int i=0;i<renderData.size();i++) {
		//Components
		Image& image = std::get<0>(renderData[i]);
		Transform& transform = std::get<1>(renderData[i]);
		//Make sure the image is loaded
		image.reload(window->getRenderer());

		//Draw loaded image
		x = int(transform.getX());
		y = int(transform.getY());
		width = image.getTexture()->getWidth();
		height = image.getTexture()->getHeight();

		SDL_Rect dest{ x, y, width, height };
		SDL_RenderCopy(window->getRenderer(), image.getTexture()->getTexture(), NULL, &dest);
	}
	//Add renderableTexts
	window->update();*/
}

/* Destroys any existing window and renderer and creates new ones */
void Graphics::createWindow() {
	//Destroy old window
	if (window != nullptr)
		delete window;

	//Create new window
	window = new Window("Hello", 0, 0, 1680, 1050, 0);
}

/* Save all components to file*/
void Graphics::write(std::ostream& os) const {
	/*int size = renderables.size();
	os.write((char*)&size, sizeof(int)); //Store amount of Components to be loaded at load time
	//store Components from vector
	for (std::vector<Renderable>::const_iterator i = renderables.begin(); i != renderables.end(); ++i) {
		//Write RenderComponent ID
		uint32_t idValues = i->render.getID();
		os.write((char*)&idValues, sizeof(uint32_t));
		//Write component
		i->render.serialize(os);

		//Write Transform ID
		idValues = i->transform.getID();
		os.write((char*)&idValues, sizeof(uint32_t));
	}*/
}

/* Loads all components from file */
void Graphics::read(std::istream& is, HandleManager* handleManager) {
	/*int size;
	is.read((char*)&size, sizeof(int));

	for (int i = 0; i < size; i++) { //Create components
		//Read component ID
		uint32_t idValues;
		is.read((char*)&idValues, sizeof(uint32_t));
		Handle renderID = Handle(idValues);

		//Read component
		RenderComponent* component;
		int componentType;
		is >> componentType;
		switch (componentType) {
			case Component::componentTypes::Image:
				component = Image::load(is);
				break;
			case Component::componentTypes::Text:
				component = Text::load(is);
				break;
			default:
				throw "Invalid Component Type during loading.";
				break;
		}
		component->setID(renderID);
		handleManager->update(renderID, renderables.size());
		//Read Transform
		is.read((char*)&idValues, sizeof(uint32_t));
		Handle transformID = Handle(idValues);
		Transform* transform = componentManager->getComponent<Transform>(transformID);

		renderables.emplace_back(transform, component);
	}*/
}