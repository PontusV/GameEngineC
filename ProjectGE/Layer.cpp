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
	renderables[renderablesSize] = renderable;
	renderablesSize++;
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

		if (l.transform.getRotation() < r.transform.getRotation()) return true;
		if (r.transform.getRotation() < l.transform.getRotation()) return false;

		if (l.texture.ID < r.texture.ID) return true;
		if (r.texture.ID < l.texture.ID) return false;

		// Draw area values
		if (!l.clipEnabled && !r.clipEnabled) return false; // They are equal, no need to check the rest

		if (l.clipEnabled < r.clipEnabled) return true;
		if (r.clipEnabled < l.clipEnabled) return false;

		// Draw rect vec4 compare
		if (l.drawRect.x < r.drawRect.x) return true;
		if (r.drawRect.x < l.drawRect.x) return false;

		if (l.drawRect.y < r.drawRect.y) return true;
		if (r.drawRect.y < l.drawRect.y) return false;

		if (l.drawRect.z < r.drawRect.z) return true;
		if (r.drawRect.z < l.drawRect.z) return false;

		if (l.drawRect.w < r.drawRect.w) return true;
		if (r.drawRect.w < l.drawRect.w) return false;

		return false; // They are equal
	});

	for (std::size_t i = 0; i < renderablesSize; i++) {
		RenderCopy2D& renderable = renderables[i];
		ConstBatchConfig config(renderable.transform.getRotation(), renderable.texture.ID, renderable.shaderID, renderable.clipEnabled, renderable.drawRect);

		if (!batch.hasRoom()) {
			batch.end();
			batch.flush();
			batch.startNewSegment(config);
		}
		if (!batch.hasBegun() || !batch.getConfig().compatible(config))
			batch.startNewSegment(config);

		batch.submit(renderable);
	}
	//End sumbit and flush
	batch.end();
	batch.flush();

	renderablesSize = 0;
}


void Layer::renderText(std::string text, Transform transform, Font font, glm::vec4 color, bool clipEnabled, const glm::vec4 drawRect) {
	Transform textTransform = transform; //Copy parent transform

	TextData2D textData	= ResourceManager::getInstance().createText(text, color, font);
	std::vector<CharTexture2D>& textTextures = textData.textures;

	// Add offset
	glm::vec2 anchor = transform.getAnchor();
	transform.setX(transform.getX() + anchor.x * textData.size.x);
	transform.setY(transform.getY() + textData.size.y + anchor.y * textData.size.y);

	//std::cout << anchor.y << ", " << textData.size.y << "\n";

	for (CharTexture2D& c : textTextures) {
		textTransform.setX(transform.getX() + c.offset.x);
		textTransform.setY(transform.getY() + c.offset.y);
		textTransform.setAnchor(TransformAnchor::TOP_LEFT);
		glm::vec2 imagePos(0, 0);
		Renderable2D renderable{ c.texture, textTransform, c.texture.size, textShaderID, color, imagePos, clipEnabled, drawRect };
		submit(renderable);
	}
}