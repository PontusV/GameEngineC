#include "Graphics.h"
#include "Window.h"
#include "data/Shader.h"
#include "engine/ResourceManager.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderable2D.h"
#include "maths/Matrix4.h"
#include "maths/Vector2.h"
#include "maths/MatrixTransform.h"

#include <algorithm>
#include <array>
#include <GLFW/glfw3.h>

#include <glad/glad.h>

using namespace Core;
Shader spriteShader;

Graphics::Graphics() : camera(&window) {}


Graphics::~Graphics() {
	if (renderer)
		delete renderer;
}

bool Graphics::createWindow(const char* windowTitle, int screenWidth, int screenHeight) {
	window = Window("Game Engine Window", 800, 600);
	if (!window.init())
		return false;

	// Successful
	return true;
}

bool Graphics::initiate() {
	if (!window.getActive()) {
		std::cout << "There is no active window!\n";
		return false;
	}
	// Load shaders
	Shader textShader = ResourceManager::getInstance().loadShader("resources/shaders/text");
	spriteShader = ResourceManager::getInstance().loadShader("resources/shaders/sprite");

	// Configure shaders
	Matrix4 projection = window.getProjectionMatrix();
	ResourceManager::getInstance().initShader(projection);

	// Initialize renderer
	renderer = new Renderer2D(&window);

	// Succesfull initialization
	return true;
}

/* Loops through Renderable vector and draws them.*/
void Graphics::render(float deltaTime) {

	// Updates Camera View Matrix
	camera.updateViewMatrix();

	renderWorld(deltaTime);
	renderer->render(deltaTime, camera.getViewMatrix());

	renderGUI(deltaTime);
	renderer->render(deltaTime);
}


void Graphics::renderWorld(float deltaTime) {
	std::size_t sizeRects = renderableRects.size();
	std::size_t sizeImages = renderableImages.size();
	std::size_t sizeTexts = renderableTexts.size();
	std::size_t sizeBorders = renderableBorders.size();
	// TODO: Submit Renderables IF they are inside the window.

	// Rect Sprites
	for (std::size_t i = 0; i < sizeRects; i++) {
		const SpriteRenderer&	renderProps		= renderableRects.get<SpriteRenderer>(i);
		const RectTransform&	transform		= renderableRects.get<RectTransform>(i);
		const RectSprite&		sprite			= renderableRects.get<RectSprite>(i);
		const Texture2D			texture			= Texture2D();
		const Color&			color			= sprite.getColor();
		const Shader&			shader			= sprite.getShader();
		unsigned char			sortingOrder	= sprite.getSortingOrder();

		if (!renderProps.isEnabled()) continue;
		renderer->submit(texture, transform, shader.ID, color, sortingOrder);
	}

	// Image Sprites
	for (std::size_t i = 0; i < sizeImages; i++) {
		const SpriteRenderer&	renderProps		= renderableImages.get<SpriteRenderer>(i);
		const RectTransform&	transform		= renderableImages.get<RectTransform>(i);
		const Image&			image			= renderableImages.get<Image>(i);
		const Texture2D&		texture			= image.getTexture();
		const Color&			color			= image.getColor();
		const Shader&			shader			= image.getShader();
		unsigned char			sortingOrder	= image.getSortingOrder();

		if (!renderProps.isEnabled()) continue;
		renderer->submit(texture, transform, shader.ID, color, sortingOrder);
	}

	// Text Sprites
	for (std::size_t i = 0; i < sizeTexts; i++) {
		const SpriteRenderer&	renderProps		= renderableTexts.get<SpriteRenderer>(i);
		const RectTransform&	transform		= renderableTexts.get<RectTransform>(i);
		Text&					text			= renderableTexts.get<Text>(i);
		const Color&			color			= text.getColor();
		const Shader&			shader			= text.getShader();
		unsigned char			sortingOrder	= text.getSortingOrder();

		if (text.isDirty()) {
			std::vector<TextSprite> sprites = createTextSprites(text, transform);
			text.update(sprites);
		}

		if (!renderProps.isEnabled()) continue;
		for (const TextSprite& sprite : text.getSprites()) {
			renderer->submit(sprite.texture, sprite.transform, shader.ID, color, sortingOrder);
		}
	}

	// Borders
	for (std::size_t i = 0; i < sizeBorders; i++) {
		const SpriteRenderer&	renderProps		= renderableBorders.get<SpriteRenderer>(i);
		const Border&			border			= renderableBorders.get<Border>(i);
		const RectTransform&	transform		= renderableBorders.get<RectTransform>(i);
		const Texture2D			texture			= Texture2D(); // No texture
		const Color&			color			= border.getColor();
		const unsigned char&	sortingOrder	= border.getSortingOrder();

		if (!renderProps.isEnabled()) continue;
		for (RectTransform& lineTransform : createBorder(border, transform)) {
			renderer->submit(texture, lineTransform, spriteShader.ID, color, sortingOrder);
		}
	}
}

void Graphics::renderGUI(float deltaTime) {
	std::size_t sizeRects = canvasRects.size();
	std::size_t sizeImages = canvasImages.size();
	std::size_t sizeTexts = canvasTexts.size();
	std::size_t sizeBorders = canvasBorders.size();

	// TODO: In GUI system -> Update Canvas and its children with proper z value for drawing order and input

	// Rect Sprites
	for (std::size_t i = 0; i < sizeRects; i++) {
		const CanvasRenderer&	renderProps		= canvasRects.get<CanvasRenderer>(i);
		const RectTransform&	transform		= canvasRects.get<RectTransform>(i);
		const RectSprite&		sprite			= canvasRects.get<RectSprite>(i);
		const Texture2D			texture			= Texture2D();
		const Color&			color			= sprite.getColor();
		const Shader&			shader			= sprite.getShader();
		unsigned char			sortingOrder	= sprite.getSortingOrder();

		if (!renderProps.isEnabled()) continue;
		renderer->submit(texture, transform, shader.ID, color, sortingOrder);
	}

	// Image Sprites
	for (std::size_t i = 0; i < sizeImages; i++) {
		const CanvasRenderer&	renderProps		= canvasImages.get<CanvasRenderer>(i);
		const RectTransform&	transform		= canvasImages.get<RectTransform>(i);
		const Image&			image			= canvasImages.get<Image>(i);
		const Texture2D&		texture			= image.getTexture();
		const Color&			color			= image.getColor();
		const Shader&			shader			= image.getShader();
		unsigned char			sortingOrder	= image.getSortingOrder();

		if (!renderProps.isEnabled()) continue;
		renderer->submit(texture, transform, shader.ID, color, sortingOrder);
	}

	// Text Sprites
	for (std::size_t i = 0; i < sizeTexts; i++) {
		const CanvasRenderer&	renderProps		= canvasTexts.get<CanvasRenderer>(i);
		const RectTransform&	transform		= canvasTexts.get<RectTransform>(i);
		Text&					text			= canvasTexts.get<Text>(i);
		const Color&			color			= text.getColor();
		const Shader&			shader			= text.getShader();
		unsigned char			sortingOrder	= text.getSortingOrder();

		if (text.isDirty()) {
			std::vector<TextSprite> sprites = createTextSprites(text, transform);
			text.update(sprites);
		}

		if (!renderProps.isEnabled()) continue;
		for (const TextSprite& sprite : text.getSprites()) {
			renderer->submit(sprite.texture, sprite.transform, shader.ID, color, sortingOrder);
		}
	}

	// Borders
	for (std::size_t i = 0; i < sizeBorders; i++) {
		const CanvasRenderer&	renderProps		= canvasBorders.get<CanvasRenderer>(i);
		const Border&			border			= canvasBorders.get<Border>(i);
		const RectTransform&	transform		= canvasBorders.get<RectTransform>(i);
		const Texture2D			texture			= Texture2D(); // No texture
		const Color&			color			= border.getColor();
		const unsigned char&	sortingOrder	= border.getSortingOrder();

		if (!renderProps.isEnabled()) continue;
		for (RectTransform& lineTransform : createBorder(border, transform)) {
			renderer->submit(texture, lineTransform, spriteShader.ID, color, sortingOrder);
		}
	}
}

void Graphics::update(float dt) {
}

Window& Graphics::getWindow() {
	return window;
}

Camera& Graphics::getCamera() {
	return camera;
}

Renderer2D& Graphics::getRenderer() {
	return *renderer;
}

std::vector<TextSprite> Graphics::createTextSprites(const Text& text, const RectTransform& transform) const {
	std::vector<TextSprite> sprites;
	if (text.getText().empty()) return sprites;

	Matrix4 localToWorldMatrix = transform.getLocalToWorldMatrix() * transform.getLocalModelMatrix();
	// Create text sprites
	TextData2D textData = ResourceManager::getInstance().getTextData(text.getText(), text.getFont());
	std::vector<CharTexture2D>& textTextures = textData.textures;
	// Calculate offset
	Vector2 pivot = transform.getPivot();
	float offsetX = textData.size.x * -pivot.x;
	float offsetY = textData.size.y * -pivot.y + textData.size.y;

	for (CharTexture2D& c : textTextures) {
		// Create new Transform for Character Sprite
		RectTransform spriteTransform(offsetX + c.offset.x, offsetY + c.offset.y, c.texture.size.x, c.texture.size.y, transform.getZ() + 0.00001f, Alignment::TOP_LEFT, 0.0f, 1.0f);
		// Set world model matrix of new text sprite
		spriteTransform.updateLocalToWorldMatrix(localToWorldMatrix);
		spriteTransform.updateLocalModelMatrix();

		sprites.push_back({ c.texture, spriteTransform });
	}
	return sprites;
}

std::vector<RectTransform> Graphics::createBorder(const Border& border, const RectTransform& transform) const {
	const std::size_t& borderThickness = border.getBorderThickness();
	bool inner = border.isInner();
	const Vector2& size = transform.getSize();

	std::vector<RectTransform> borderLines;

	for (std::size_t side = 0; side < 4; side++) { // 4 lines
		if (border.sideEnabled(side)) { // Check if this side is enabled (0 = top, 1 = right, 2 = bottom, 3 = left)

			Vector2 rectSize = size;
			if (!inner) {
				rectSize.x += borderThickness * 2;
				rectSize.y += borderThickness * 2;
			}

			float localPosX = rectSize.x * -transform.getPivot().x; // Init with offset
			float localPosY = rectSize.y * -transform.getPivot().y; // Initi with offset


			if (side == 1) {		// Right
				localPosX += (float)rectSize.x - borderThickness;
				localPosY += borderThickness;
			}
			else if (side == 2) {	// Bottom
				localPosY += (float)rectSize.y - borderThickness;
			}
			else if (side == 3) {	// Left
				localPosY += borderThickness;
			}

			Vector2 borderSize;
			if (side == 0 || side == 2) {	// Vertical line
				borderSize.x = rectSize.x;
				borderSize.y = borderThickness;
			}
			else {						// Horizontal line
				borderSize.x = borderThickness;
				borderSize.y = rectSize.y - borderThickness * 2;
			}

			RectTransform lineTransform = RectTransform(localPosX, localPosY, borderSize.x, borderSize.y, transform.getZ(), Alignment::TOP_LEFT, 0.0f, 1.0f);
			lineTransform.updateLocalToWorldMatrix(transform.getLocalToWorldMatrix() * transform.getLocalModelMatrix());
			lineTransform.updateLocalModelMatrix();
			borderLines.push_back(lineTransform);
		}
	}

	return borderLines;
}

IWindow* Graphics::getWindowInterface() {
	return &getWindow();
}

ICamera* Graphics::getCameraInterface() {
	return &getCamera();
}