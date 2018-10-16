#include "ResourceManager.h"
#include "Image.h"
#include "Text.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <sstream>
#include <SDL_image.h>


ResourceManager::ResourceManager() {
} //Constructor
ResourceManager::~ResourceManager() {
} //Destructor

ResourceManager& ResourceManager::getInstance() {
	static ResourceManager instance; //Instantiated on first use.
	return instance;
}

TexturePtr ResourceManager::loadTexture(const std::string& fileName, SDL_Renderer* ren) {
	//Search for existing TexturePtr with given key
	std::map<std::string, TexturePtr>::iterator it;
	it = loadedTextures.find(fileName);

	if (it != loadedTextures.end())
		return it->second;

	//No existing TexturePtr found with given key
	//Creates a new TexturePtr and returns it
	SDL_Surface* image = IMG_Load(fileName.c_str());
	Texture* texture = Texture::load(image, ren);
	TexturePtr tx(texture);
	loadedTextures.insert(std::make_pair(fileName, tx)); //Stores newly created texture in map with given key

	return tx;
}

TexturePtr ResourceManager::loadText(const std::string& text, Font& font, SDL_Color color, SDL_Renderer* ren) {
	std::ostringstream oss;
	oss << text << "_" << font.getFileName() << "_" << font.getSize() << "_" << color.r << "_" << color.b << "_" << color.g << "_" << color.a; //Generates unique key from parameters
	std::string key = oss.str();

	//Search for existing TexturePtr with given key
	std::map<std::string, TexturePtr>::iterator it;
	it = loadedTextures.find(key);

	if (it != loadedTextures.end())
		return it->second;

	//No existing TexturePtr found with given key
	//Creates a new TexturePtr and returns it
	Texture* texture = Text::loadTexture(text, font, color, ren);
	TexturePtr tx(texture);
	loadedTextures.insert(std::make_pair(key, tx)); //Stores newly created texture in map with given key
	return tx;
}