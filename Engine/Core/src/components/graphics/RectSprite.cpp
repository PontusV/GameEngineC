#include "RectSprite.h"
#include "engine/ResourceManager.h"

using namespace Core;

RectSprite::RectSprite(std::size_t sortingOrder, Color color, Shader shader) : Graphic(sortingOrder, shader, color) {}
RectSprite::RectSprite(std::size_t sortingOrder, Color color) : Graphic(sortingOrder, ResourceManager::getInstance().loadShader("Shaders/sprite"), color) {}
RectSprite::RectSprite() {}
RectSprite::~RectSprite() {}