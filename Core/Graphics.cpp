#include "Graphics.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Renderable2D.h"
#include "Sprite.h"
#include "Maths/Matrix4.h"
#include "Maths/Vector2.h"
#include "Maths/MatrixTransform.h"

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
	if (!window.isActive()) {
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
	std::size_t sizeRectangles		= renderableRects.rects.size();
	std::size_t sizeImages			= renderableImages.images.size();
	std::size_t sizeTexts			= renderableTexts.texts.size();
	std::size_t sizeTexturedSprites	= renderableTexturedSprites.sprites.size();
	std::size_t sizeBorders			= renderableBorders.borders.size();

	// Updates Camera View Matrix
	ResourceManager::getInstance().updateCameraViewMatrix(camera.getViewMatrix());

	// TODO: Add Renderables to vector IF they are inside the window. No need to sort Renderables if they cant be drawn in camera view
	// Rectangles
	for (std::size_t i = 0; i < sizeRectangles; i++) {
		const RectSprite&		rect			= renderableRects.rects[i];
		const RectTransform&	transform		= renderableRects.transforms[i];
		const Texture2D			texture			= Texture2D(); // No texture
		const Color&			color			= rect.getColor();
		unsigned char			layerIndex		= rect.getOwner().getEntityLayer();
		const std::vector<std::array<Vector2, 4>>& masks = rect.getMasks();

		renderer->submit(texture, transform, spriteShader.ID, color, masks, layerIndex);
	}

	// Images (Reload)
	for (std::size_t i = 0; i < sizeImages; i++) {
		renderableImages.images[i].reload(); //Make sure image is loaded
	}

	static Shader textShader = ResourceManager::getInstance().loadShader("resources/shaders/text");
	// Texts
	for (std::size_t i = 0; i < sizeTexts; i++) {
		const Text& text				= renderableTexts.texts[i];
		const unsigned char& layerIndex = text.getOwner().getEntityLayer();
		const std::vector<std::array<Vector2, 4>> & masks = text.getMasks();

		const std::vector<RectTransform>& textTransforms = text.getTextTransforms();
		const std::vector<Texture2D>& textSprites = text.getTextSprites();
		for (std::size_t i2 = 0; i2 < textSprites.size(); i2++) {
			renderer->submit(textSprites[i2], textTransforms[i2], textShader.ID, text.getColor(), masks, layerIndex);
		}
	}

	// Textured Sprites
	for (std::size_t i = 0; i < sizeTexturedSprites; i++) {
		const TexturedSprite&	sprite				= renderableTexturedSprites.sprites[i];
		const RectTransform&	transform			= renderableTexturedSprites.transforms[i];
		const Texture2D&		texture				= sprite.getTexture();
		const Color&			color				= sprite.getColor();
		unsigned char			layerIndex			= sprite.getOwner().getEntityLayer();
		const std::vector<std::array<Vector2, 4>> & masks = sprite.getMasks();

		renderer->submit(texture, transform, sprite.getShader().ID, color, masks, layerIndex);
	}

	// Borders
	for (std::size_t i = 0; i < sizeBorders; i++) {
		const Border&			border			= renderableBorders.borders[i];
		const RectTransform&	transform		= renderableBorders.transforms[i];
		const Texture2D			texture			= Texture2D(); // No texture
		const Color&			color			= border.getColor();
		const std::size_t&		borderThickness	= border.getBorderThickness();
		bool					inner			= border.isInner();
		const Vector2&			size			= transform.getSize();
		const unsigned char&	layerIndex		= border.getOwner().getEntityLayer();
		const std::vector<std::array<Vector2, 4>>& masks	= border.getMasks();


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
				} else {						// Horizontal line
					borderSize.x = borderThickness;
					borderSize.y = rectSize.y - borderThickness * 2;
				}

				RectTransform lineTransform = RectTransform(localPosX, localPosY, borderSize.x, borderSize.y, transform.getZ(), Alignment::TOP_LEFT, 0.0f, 1.0f);
				lineTransform.updateLocalToWorldMatrix(transform.getLocalToWorldMatrix() * transform.getLocalModelMatrix());

				renderer->submit(texture, lineTransform, spriteShader.ID, color, masks, layerIndex);
			}
		}
	}

	// Clear, Draw & Update window
	window.clear();
	renderer->render(deltaTime);
	window.update();
}

void Graphics::update(float dt) {
	// Update UI
	userInterfaceSystem.update();
}

unsigned char Graphics::createLayer() {
	return renderer->createLayer();
}

Window& Graphics::getWindow() {
	return window;
}

Renderer2D& Graphics::getRenderer() {
	return *renderer;
}

UISystem& Graphics::getUISystem() {
	return userInterfaceSystem;
}