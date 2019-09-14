#include "TexturedSprite.h"
#include "ResourceManager.h"
using namespace Core;

TexturedSprite::TexturedSprite(Texture2D texture, Shader shader, Color color) : Sprite(color), shader(shader), texture(texture) {
}

TexturedSprite::TexturedSprite(Texture2D texture, Color color) : Sprite(color), texture(texture), shader(ResourceManager::getInstance().loadShader("resources/shaders/sprite")) {
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