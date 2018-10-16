#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <memory>
#include <string>
#include <map>
#include "Texture.h"
#include "Font.h"

typedef std::shared_ptr<Texture> TexturePtr;

struct SDL_Renderer;


class ResourceManager {
public:
	static ResourceManager& getInstance();
	~ResourceManager();
	//Makes objects of this class Noncopyable (important due to singleton)
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//Load functions
	TexturePtr loadTexture(const std::string& fileName, SDL_Renderer* ren);
	TexturePtr loadText(const std::string& text, Font& font, SDL_Color color, SDL_Renderer* ren);
private:
	ResourceManager();
	std::map<std::string, TexturePtr> loadedTextures;
};
#endif