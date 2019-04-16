#include "Layer.h"
#include "ResourceManager.h"
#include <algorithm>

using namespace Core;

Layer::Layer(int index, Window* window) : index(index), batch(window)
{
	batch.init();
	textShaderID = ResourceManager::getInstance().loadShader("resources/shaders/text.vert", "resources/shaders/text.frag").ID;
	renderablesSize = 0;
}


Layer::~Layer()
{
}

void Layer::submit(const Renderable2D& renderable) {
	if (renderablesSize < MAX_RENDERABLES_PER_LAYER) {
		renderables[renderablesSize] = renderable;
		renderablesSize++;
	}
	else {
		std::cout << "Layer renderable buffer is full. Cannot add any more renderables!\n";
		throw std::invalid_argument("Layer renderable buffer is full. Cannot add any more renderables!");
	}
}

void Layer::flush() {
	if (renderablesSize == 0)
		return;

	// Sort list of renderable copies
	std::sort(std::begin(renderables), &renderables[renderablesSize], [](RenderCopy2D& l, RenderCopy2D& r) {
		
		if (l.transform.getZ() < r.transform.getZ()) return true;
		if (r.transform.getZ() < l.transform.getZ()) return false;

		if (l.shaderID > r.shaderID) return true;
		if (r.shaderID > l.shaderID) return false;

		if (l.texture.ID > r.texture.ID) return true;
		if (r.texture.ID > l.texture.ID) return false;

		// Clipping values
		if (!l.clipEnabled && !r.clipEnabled) return false; // They are equal, no need to check the rest

		if (l.clipEnabled < r.clipEnabled) return true;
		if (r.clipEnabled < l.clipEnabled) return false;

		// Clip Mask Vertices compare
		//if (l.clipMaskVertices < r.clipMaskVertices) return true;
		//if (l.clipMaskVertices > r.clipMaskVertices) return false;

		return false; // They are equal
	});

	bool batchBegun = false;
	for (std::size_t i = 0; i < renderablesSize; i++) {
		RenderCopy2D& renderable = renderables[i];
		ConstBatchConfig config(renderable.texture.ID, renderable.shaderID, renderable.clipEnabled, renderable.clipMaskVertices);

		if (!batch.hasRoom()) {
			batch.end();
			batch.flush();
			batchBegun = false;
		}

		if (!batchBegun) {
			batch.startNewSegment(config);
			batch.begin();
			batchBegun = true;
		} else if (!batch.getConfig().compatible(config)) {
			batch.startNewSegment(config);
		}

		batch.submit(renderable);
	}
	//End sumbit and flush
	batch.end();
	batch.flush();

	renderablesSize = 0;
}


void Layer::renderText(std::string text, Transform transform, Font font, glm::vec4 color, bool clipEnabled, const std::vector<glm::vec2>& clipMaskVertices) {
	TextData2D textData	= ResourceManager::getInstance().createText(text, color, font);
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

		Renderable2D renderable{ c.texture, spriteTransform, c.texture.size, textShaderID, color, clipEnabled, clipMaskVertices };
		submit(renderable);
	}
}