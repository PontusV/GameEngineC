#include "Image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;

Image::Image(const char* fileName, Color color) : TexturedSprite(ResourceManager::getInstance().loadTexture(fileName), ResourceManager::getInstance().loadShader("resources/shaders/sprite"), color), fileName(fileName) {
} // Constructor

Image::Image(const char* fileName, Shader shader, Color color) : TexturedSprite(ResourceManager::getInstance().loadTexture(fileName), shader, color), fileName(fileName) {
} // Constructor

Image::Image() : TexturedSprite(Texture2D(), ResourceManager::getInstance().loadShader("resources/shaders/sprite")) {
}

Image::~Image() {
} // Destructor

void Image::reload(bool force) {
	if (getTexture().ID == 0 || force) {
		setTexture(ResourceManager::getInstance().loadTexture(fileName.c_str()));
	}
}

void Image::set(const char* fileName) {
	this->fileName = fileName;
	reload(true);
}

const char* Image::getFileName() const {
	return fileName.c_str();
}