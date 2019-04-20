#include "Renderer2D.h"
#include "ResourceManager.h"
#include "BatchConfig.h"
#include "TransformMaths.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace Core;

Renderer2D::Renderer2D(Window* window) : window(window), batch(window), postProcessor(window) {
	batch.init();
	textShaderID = ResourceManager::getInstance().loadShader("resources/shaders/text.vert", "resources/shaders/text.frag").ID;
}

Renderer2D::~Renderer2D() {
}

unsigned char Renderer2D::createLayer() {
	return layerAmount++;
}

void Renderer2D::submit(const Texture2D& texture, const Transform& transform, const glm::ivec2& size, const unsigned int& shaderID, const glm::vec4& color, const bool& clipEnabled, const std::vector<glm::vec2>& clipMaskVertices, const unsigned char& layerIndex) {
	if (clipMaskVertices.size() % 4 != 0) throw std::invalid_argument("Invalid amount of Clip Mask vertices!");
	if (clipEnabled && clipMaskVertices.size() < 4) throw std::invalid_argument("Too few Clip Mask vertices!");

	const glm::vec2& anchor = transform.getAnchor();
	const glm::mat4& localToWorldMatrix = transform.getLocalToWorldMatrix();
	const glm::vec2& rSize = size;

	Renderable2D& renderable = renderableBuffer[renderablesSize];
	renderable.textureID = texture.ID;

	renderable.vertices[0] = localToWorldMatrix * (rSize * anchor);
	renderable.vertices[1] = localToWorldMatrix * (glm::vec2(0, rSize.y) + rSize * anchor);
	renderable.vertices[2] = localToWorldMatrix * (rSize + rSize * anchor);
	renderable.vertices[3] = localToWorldMatrix * (glm::vec2(rSize.x, 0) + rSize * anchor);

	std::copy(std::begin(texture.uvCoords), std::end(texture.uvCoords), std::begin(renderable.uvCoords)); // uv coords
	renderable.z = transform.getZ();
	renderable.shaderID = shaderID;
	renderable.color = Color(color.x, color.y, color.z, color.a);
	renderable.layerIndex = layerIndex;

	renderable.clipEnabled = clipEnabled;
	renderable.clipMaskVertices = clipMaskVertices;

	renderablesSize++;
}

void Renderer2D::render(float deltaTime) {
	postProcessor.begin();
	flush();
	postProcessor.end();
	postProcessor.render(deltaTime);
}

/* Sort all Renderables and submit to BatchRenderer. */
void Renderer2D::flush() {
	if (renderablesSize == 0)
		return;

	// Sort list of renderable copies
	std::sort(std::begin(renderableBuffer), &renderableBuffer[renderablesSize], [](Renderable2D& l, Renderable2D& r) {

		if (l.layerIndex < r.layerIndex) return true;
		if (r.layerIndex < l.layerIndex) return false;

		if (l.z < r.z) return true;
		if (r.z < l.z) return false;

		if (l.shaderID > r.shaderID) return true;
		if (r.shaderID > l.shaderID) return false;

		if (l.textureID > r.textureID) return true;
		if (r.textureID > l.textureID) return false;

		// Clipping values
		if (!l.clipEnabled && !r.clipEnabled) return false; // They are equal, no need to check the rest

		if (l.clipEnabled < r.clipEnabled) return true;
		if (r.clipEnabled < l.clipEnabled) return false;

		// Stencil Buffer Clip Mask Vertices compare
		std::size_t vertexAmount = std::min(l.clipMaskVertices.size(), r.clipMaskVertices.size());
		for (std::size_t i = 0; i < vertexAmount; i++) {
			glm::vec2 lVertex = l.clipMaskVertices[i];
			glm::vec2 rVertex = r.clipMaskVertices[i];

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
		ConstBatchConfig config(renderable.textureID, renderable.shaderID, renderable.clipEnabled, renderable.clipMaskVertices);

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

void Renderer2D::submitText(const std::string& text, const Transform& transform, const Font& font, const glm::vec4& color, const bool& clipEnabled, const std::vector<glm::vec2>& clipMaskVertices, const unsigned int& layerIndex) {
	TextData2D textData = ResourceManager::getInstance().createText(text, color, font);
	std::vector<CharTexture2D>& textTextures = textData.textures;

	// Calculate offset
	glm::vec2 anchor = transform.getAnchor();
	float offsetX = anchor.x * textData.size.x;
	float offsetY = textData.size.y + anchor.y * textData.size.y;

	for (CharTexture2D& c : textTextures) {
		// Create new Transform for Character Sprite
		Transform spriteTransform(offsetX + c.offset.x, offsetY + c.offset.y, transform.getZ(), TransformAnchor::TOP_LEFT, 0.0f, 1.0f);
		// Set world model matrix of new text sprite
		spriteTransform.updateLocalToWorldMatrix(transform.getLocalToWorldMatrix() * spriteTransform.getLocalModelMatrix());

		//Renderable2D renderable{ c.texture, spriteTransform, c.texture.size, textShaderID, color, clipEnabled, clipMaskVertices };
		//submit(renderable, layerIndex);
		submit(c.texture, spriteTransform, c.texture.size, textShaderID, color, clipEnabled, clipMaskVertices, layerIndex);
	}
}

void Renderer2D::updateSize(unsigned int width, unsigned int height) {
	postProcessor.setSize(width, height);
}