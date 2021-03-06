#include "Image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;


Image::Image(const char* fileName, unsigned char layerIndex) : Sprite(layerIndex), fileName(fileName) {
} // Constructor

Image::Image() {
} 

Image::~Image() {
} // Destructor

void Image::reload() {
	if (texture.ID == 0) {
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

// ------------------------------- Serializable ----------------------------------------

void Image::serialize(std::ostream& os) const {
	Sprite::serialize(os);

	os.write(fileName.c_str(), fileName.size() + 1);		// File name
}
void Image::deserialize(std::istream& is) {
	Sprite::deserialize(is);

	std::getline(is, fileName, '\0');						// File name
}