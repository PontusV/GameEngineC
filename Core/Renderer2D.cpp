#include "Renderer2D.h"
#include "ResourceManager.h"
#include "BatchConfig.h"
#include "Maths/MatrixTransform.h"
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

unsigned char Renderer2D::createLayer() {
	return layerAmount++;
}

void Renderer2D::submit(const Texture2D& texture, const RectTransform& transform, const unsigned int& shaderID, const Color& color, const std::vector<std::array<Vector2, 4>>& masks, const unsigned char& layerIndex) {
	if (renderablesSize >= MAX_RENDERABLES) {
		// TODO: Add warning to increase MAX_RENDERABLES
		return;
	}

	std::vector<Vector2> clipMaskVertices;
	clipMaskVertices.reserve(masks.size()*4);
	for (const std::array<Vector2, 4>& vertices : masks) {
		for (std::size_t i = 0; i < 4; i++) {
			clipMaskVertices.push_back(vertices[i]);
		}
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
	renderable.layerIndex = layerIndex;

	renderable.clipMaskVertices = clipMaskVertices;

	renderablesSize++;
}

void Renderer2D::render(float deltaTime) {
	postProcessor.begin();
	flush();
	postProcessor.end();
	postProcessor.render(deltaTime);
}

void Renderer2D::flush() {
	if (renderablesSize == 0)
		return;

	// Sort list of renderable copies
	std::sort(std::begin(renderableBuffer), &renderableBuffer[renderablesSize], [](Renderable2D& l, Renderable2D& r) {

		if (l.layerIndex > r.layerIndex) return true;
		if (r.layerIndex > l.layerIndex) return false;

		if (l.z < r.z) return true;
		if (r.z < l.z) return false;

		if (l.shaderID > r.shaderID) return true;
		if (r.shaderID > l.shaderID) return false;

		if (l.textureID > r.textureID) return true;
		if (r.textureID > l.textureID) return false;

		// Clipping values
		if (l.clipMaskVertices.size() == 0 && !r.clipMaskVertices.size() == 0) return false; // They are equal, no need to check the rest

		if (l.clipMaskVertices.size() == 0 && r.clipMaskVertices.size() != 0) return true;
		if (r.clipMaskVertices.size() == 0 && l.clipMaskVertices.size() != 0) return false;

		// Stencil Buffer Clip Mask Vertices compare
		std::size_t vertexAmount = std::min(l.clipMaskVertices.size(), r.clipMaskVertices.size());
		for (std::size_t i = 0; i < vertexAmount; i++) {
			Vector2 lVertex = l.clipMaskVertices[i];
			Vector2 rVertex = r.clipMaskVertices[i];

			if (lVertex.x < rVertex.x) return true;
			if (rVertex.x < lVertex.x) return false;
			if (lVertex.y < rVertex.y) return true;
			if (rVertex.y < lVertex.y) return false;
		}

		if (l.clipMaskVertices.size() < vertexAmount) return true;
		if (r.clipMaskVertices.size() < vertexAmount) return false;

		return false; // They are equal
	});

	bool batchBegun = false;
	for (std::size_t i = 0; i < renderablesSize; i++) {
		Renderable2D& renderable = renderableBuffer[i];
		ConstBatchConfig config(renderable.textureID, renderable.shaderID, renderable.clipMaskVertices);

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

	renderablesSize = 0;
}

void Renderer2D::submitText(const std::wstring& text, const RectTransform& transform, const Font& font, const Color& color, const std::vector<std::array<Vector2, 4>>& clipMaskVertices, const unsigned int& layerIndex) {
	TextData2D textData = ResourceManager::getInstance().createText(text, font);
	std::vector<CharTexture2D>& textTextures = textData.textures;

	// Calculate offset
	Vector2 pivot = transform.getPivot();
	float offsetX = textData.size.x * -pivot.x;
	float offsetY = textData.size.y * -pivot.y + textData.size.y;

	for (CharTexture2D& c : textTextures) {
		// Create new Transform for Character Sprite
		RectTransform spriteTransform(offsetX + c.offset.x, offsetY + c.offset.y, c.texture.size.x, c.texture.size.y, transform.getZ(), Alignment::TOP_LEFT, 0.0f, 1.0f);
		// Set world model matrix of new text sprite
		spriteTransform.updateLocalToWorldMatrix(transform.getLocalToWorldMatrix() * transform.getLocalModelMatrix());
		submit(c.texture, spriteTransform, textShaderID, color, clipMaskVertices, layerIndex);
	}
}

void Renderer2D::updateSize(unsigned int width, unsigned int height) {
	postProcessor.setSize(width, height);
}