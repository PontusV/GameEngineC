#include "TexturedSprite.h"
#include "engine/ResourceManager.h"
using namespace Core;

TexturedSprite::TexturedSprite(Texture2D texture, Shader shader, Color color, std::size_t sortingLayer) : Sprite(color, sortingLayer), shader(shader), texture(texture) {
}

TexturedSprite::TexturedSprite(Texture2D texture, Color color, std::size_t sortingLayer) : Sprite(color, sortingLayer), texture(texture), shader(ResourceManager::getInstance().loadShader("resources/shaders/sprite")) {
}

TexturedSprite::TexturedSprite() : shader(ResourceManager::getInstance().loadShader("resources/shaders/sprite")) {
}

TexturedSprite::~TexturedSprite() {
}

void TexturedSprite::setTexture(Texture2D texture) {
	this->texture = texture;
}

const Texture2D& TexturedSprite::getTexture() const {
	return texture;
}

void TexturedSprite::setShader(Shader shader) {
	this->shader = shader;
}

const Shader& TexturedSprite::getShader() const {
	return shader;
}