#include "Image.h"
#include "engine/ResourceManager.h"

using namespace Core;

Image::Image(const char* fileName, const char* shaderPath, Color color)
	: fileName(fileName), texture(ResourceManager::getInstance().loadTexture(fileName)), Graphic(0, shaderPath, color) {
} // Constructor
Image::Image(const char* fileName, Color color)
	: fileName(fileName), texture(ResourceManager::getInstance().loadTexture(fileName)), Graphic(0, "Shaders/sprite", color) {
} // Constructor

Image::Image() {
}

Image::~Image() {
} // Destructor

void Image::reload(bool force) {
	if (!fileName.empty() && texture.ID == 0 || force) {
		texture = ResourceManager::getInstance().loadTexture(fileName.c_str());
	}
}

void Image::set(const char* fileName) {
	this->fileName = fileName;
	reload(true);
}

const char* Image::getFileName() const {
	return fileName.c_str();
}

const Texture2D& Image::getTexture() const {
	return texture;
}

bool Image::isDirty() const {
	return dirty;
}

void Image::setDirty(bool value) {
	dirty = value;
}