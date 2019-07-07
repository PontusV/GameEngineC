#include "Image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;

Image::Image(const char* fileName, unsigned char layerIndex) : Sprite(layerIndex), fileName(fileName) {
} // Constructor

Image::Image(const char* fileName, unsigned char layerIndex, Shader shader) : Sprite(layerIndex, {255,255,255,255}, shader), fileName(fileName) {
} // Constructor

Image::Image() {
}

Image::~Image() {
} // Destructor

void Image::reload(bool force) {
	if (texture.ID == 0 || force) {
		
		texture = ResourceManager::getInstance().loadTexture(fileName.c_str());
	}
}

Texture2D& Image::getTexture() {
	return texture;
}

void Image::setTexture(const char* fileName, Texture2D& texture) {
	this->fileName = fileName;
	this->texture = texture;
}

const char* Image::getFileName() const {
	return fileName.c_str();
}