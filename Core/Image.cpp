#include "Image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;

Image::Image(const char* fileName, unsigned char layerIndex) : Sprite(layerIndex), fileName(fileName) {
	// Default shader
	shader = ResourceManager::getInstance().loadShader("resources/shaders/sprite");
} // Constructor

Image::Image(const char* fileName, unsigned char layerIndex, Shader shader, Color color) : Sprite(layerIndex, color), fileName(fileName), shader(shader) {
} // Constructor

Image::Image() {
	// Default shader
	shader = ResourceManager::getInstance().loadShader("resources/shaders/sprite");
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

const Shader& Image::getShader() const {
	return shader;
}

void Image::setShader(Shader shader) {
	this->shader = shader;
}