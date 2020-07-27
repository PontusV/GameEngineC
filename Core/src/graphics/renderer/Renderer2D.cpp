#include "Renderer2D.h"
#include "engine/ResourceManager.h"
#include "BatchConfig.h"
#include "maths/MatrixTransform.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace Core;

Renderer2D::Renderer2D(Window* window) : batch(window), postProcessor(window) {
	batch.init();
	textShaderID = ResourceManager::getInstance().loadShader("resources/shaders/text").ID;
}

Renderer2D::~Renderer2D() {
}

void Renderer2D::submit(const Texture2D& texture, const RectTransform& transform, const unsigned int& shaderID, const Color& color, const unsigned char& sortingOrder) {
	if (renderablesSize >= MAX_RENDERABLES) {
		// TODO: Add warning to increase MAX_RENDERABLES
		return;
	}

	Renderable2D& renderable = renderableBuffer[renderablesSize];
	renderable.textureID = texture.ID;

	auto vertices = transform.getVertices();
	renderable.vertices[0] = vertices[0];
	renderable.vertices[1] = vertices[1];
	renderable.vertices[2] = vertices[2];
	renderable.vertices[3] = vertices[3];

	std::copy(std::begin(texture.uvCoords), std::end(texture.uvCoords), std::begin(renderable.uvCoords)); // uv coords
	renderable.z = transform.getZ();
	renderable.shaderID = shaderID;
	renderable.color = color;
	renderable.sortingOrder = sortingOrder;
	renderablesSize++;
}

void Renderer2D::render(float deltaTime, Matrix4 viewMatrix) {
	ResourceManager::getInstance().updateShaderViewMatrix(viewMatrix);
	//postProcessor.begin();
	flushAll();
	//postProcessor.end();
	//postProcessor.render(deltaTime);
}

void Renderer2D::flushAll() {
	if (renderablesSize > 0) {
		flush(0, renderablesSize - 1);
	}
	renderablesSize = 0;
}

void Renderer2D::flush(std::size_t startIndex, std::size_t endIndex) {
	// Sort list of renderable copies
	std::sort(&renderableBuffer[startIndex], &renderableBuffer[endIndex], [](Renderable2D& l, Renderable2D& r) {

		if (l.z < r.z) return true;
		if (r.z < l.z) return false;

		if (l.sortingOrder < r.sortingOrder) return true;
		if (r.sortingOrder < l.sortingOrder) return false;

		if (l.shaderID > r.shaderID) return true;
		if (r.shaderID > l.shaderID) return false;

		if (l.textureID > r.textureID) return true;
		if (r.textureID > l.textureID) return false;

		return false; // They are equal
		});

	bool batchBegun = false;
	for (std::size_t i = startIndex; i <= endIndex; i++) {
		Renderable2D& renderable = renderableBuffer[i];
		ConstBatchConfig config(renderable.textureID, renderable.shaderID);

		if (!batch.hasRoom()) {
			batch.end();
			batch.flush();
			batchBegun = false;
		}

		if (!batchBegun) {
			batch.startNewSegment(config);
			batch.begin();
			batchBegun = true;
		}
		else if (!batch.getConfig().compatible(config)) {
			batch.startNewSegment(config);
		}

		batch.submit(renderable);
	}
	//End sumbit and flush
	batch.end();
	batch.flush();
}

void Renderer2D::updateSize(unsigned int width, unsigned int height) {
	postProcessor.setSize(width, height);
}