#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include "Component.h"
#include "HandleManager.h"
#include "ResourceManager.h"

class Texture;

class Image : public Component {
public:
	static const int TYPE_ID = HandleManager::HANDLE_TYPE_COMPONENT_IMAGE; //Variable used for Entity::GetComponent()

	Image(const std::string fileName);
	Image(std::istream& is);

	~Image();
	void reload(SDL_Renderer* ren);
	TexturePtr getTexture();

	//Save & Load operator
	void serialize(std::ostream& os) const;
	void deserialize(std::istream& is);
private:
	//static SDL_Texture* loadImage(const char* fileName, SDL_Renderer* ren);
	Image();

	std::string fileName; //std::string probably has varying size. Change type!!!
	TexturePtr texture;
};
#endif