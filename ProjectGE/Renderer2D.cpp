#include "Renderer2D.h"
#include "ResourceManager.h"
#include "BatchConfig.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace GameEngine;

Renderer2D::Renderer2D()
{
	textShaderID = ResourceManager::getInstance().loadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs").ID;
	batches.reserve(MAX_BATCH_AMOUNT);
	push(glm::mat4(1.0f), true); //Push identity matrix
}


Renderer2D::~Renderer2D()
{
}
void Renderer2D::push(glm::mat4 matrix, bool override) {
	if (override)
		transformationStack.push_back(matrix);
	else
		transformationStack.push_back(transformationStack.back() * matrix);
	transformationStackBack = &transformationStack.back();
}

void Renderer2D::pop() {
	if (transformationStack.size() > 1) {
		transformationStack.pop_back();
		transformationStackBack = &transformationStack.back();
	}
}

void Renderer2D::submit(const Renderable2D& renderable, const unsigned short layer) {
	ConstBatchConfig config(layer, renderable.transform.getRotation(), renderable.texture.ID, renderable.shaderID);

	for (std::size_t i = 0; i < MAX_BATCH_AMOUNT; i++) { //Check batch with matching config and if it has room
		BatchRenderer2D& batch = batchStorage[i];
		if (!batch.hasBegun()) { // Take unused
			batch.init(config);

			batch.begin();
			batch.submit(renderable);
			batches.push_back(&batch);
			return;
		} else if (batch.getConfig().compatible(config) && batch.hasRoom()) { // Match found
			batch.submit(renderable);
			return;
		}
	}
	throw std::length_error("Not enough batches in storage!");
}

/* Draws and clears list of batches. */
void Renderer2D::flush() {
	//SORT BATCHES BY LAYER
	std::sort(batches.begin(), batches.end(), [](BatchRenderer2D* l, BatchRenderer2D* r) { //Highest layer infront
		return l->getConfig().layer > r->getConfig().layer;
	});
	//End sumbit and flush
	for (std::size_t i = 0; i < batches.size(); i++) {
		batches[i]->end();
		batches[i]->flush();
	}
	//Clear active batch vec
	batches.clear();
}


void Renderer2D::renderText(std::string text, Transform transform, Font font, glm::vec4 color, const unsigned int layer) {
	Transform textTransform = transform; //Copy parent transform

	std::vector<CharTexture2D> textTextures = ResourceManager::getInstance().createText(text, color, font);
	for (CharTexture2D& c : textTextures) {
		textTransform.setX(transform.getX() + c.offset.x);
		textTransform.setY(transform.getY() + c.offset.y);
		Renderable2D renderable{ c.texture, textTransform, c.texture.size, textShaderID, color };
		submit(renderable, layer);
	}
}