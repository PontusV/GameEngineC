#include "Graphics.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Renderable2D.h"
#include "TransformMaths.h"

#include <algorithm>
#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Core;
Shader spriteShader;

Graphics::Graphics() {}


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
	Shader textShader = ResourceManager::getInstance().loadShader("resources/shaders/text.vert", "resources/shaders/text.frag");
	spriteShader = ResourceManager::getInstance().loadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(window.getWidth()), static_cast<GLfloat>(window.getHeight()), 0.0f, -1.0f, 1.0f);
	GLint texIDs[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	};
	// Sprite shader
	spriteShader.setIntegerV("textures", 32, texIDs, true);
	spriteShader.setMatrix4("projection", projection);
	// Text shader
	textShader.setIntegerV("textures", 32, texIDs, true);
	textShader.setMatrix4("projection", projection);
	
	// Initialize renderer
	renderer = new Renderer2D(&window);

	// Succesfull initialization
	return true;
}

/* Loops through Renderable vector and draws them.*/
void Graphics::render() {
	std::size_t sizeRectangles	= renderableRects.rects.size();
	std::size_t sizeImages		= renderableImages.images.size();
	std::size_t sizeTexts		= renderableTexts.texts.size();
	std::size_t sizeBorders		= renderableBorders.borders.size();

	// TODO: Add Renderables to vector IF they are inside the window. No need to sort Renderables if they cant be drawn in camera view
	// Rectangles
	for (std::size_t i = 0; i < sizeRectangles; i++) {
		const Rect&			rect			= renderableRects.rects[i];
		const Transform&	transform		= renderableRects.transforms[i];
		const Texture2D		texture			= Texture2D(); // No texture
		const glm::vec4&	color			= rect.getColor();
		const glm::ivec2&	size			= rect.getSize();
		bool				clipEnabled		= rect.isClipEnabled();
		unsigned short		layerIndex		= rect.getLayerIndex();
		const std::vector<glm::vec2>& clipMaskVertices = rect.getClipMaskVertices();

		renderer->submit({ texture, transform, size, spriteShader.ID, color, clipEnabled, clipMaskVertices }, layerIndex);
		// Reset clipping
		renderableRects.rects[i].resetClipping();
	}

	// Images
	for (std::size_t i = 0; i < sizeImages; i++) {
		renderableImages.images[i].reload(); //Make sure image is loaded

		Image&				image		= renderableImages.images[i];
		const Transform&	transform	= renderableImages.transforms[i];
		const Texture2D&	texture		= image.getTexture();
		const glm::vec4		color		= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		bool				clipEnabled = image.isClipEnabled();
		unsigned short		layerIndex	= image.getLayerIndex();
		const std::vector<glm::vec2>& clipMaskVertices = image.getClipMaskVertices();

		renderer->submit({ texture, transform, texture.size, spriteShader.ID, color, clipEnabled, clipMaskVertices }, layerIndex);
		// Reset clipping
		renderableImages.images[i].resetClipping();
	}

	// Texts
	for (std::size_t i = 0; i < sizeTexts; i++) {
		const Text&			text		= renderableTexts.texts[i];
		bool				clipEnabled = text.isClipEnabled();
		unsigned short		layerIndex	= text.getLayerIndex();
		const std::vector<glm::vec2>& clipMaskVertices = text.getClipMaskVertices();

		renderer->renderText(text.getText(), renderableTexts.transforms[i], text.getFont(), text.getColor(), clipEnabled, clipMaskVertices, layerIndex);
		// Reset clipping
		renderableTexts.texts[i].resetClipping();
	}

	// Borders
	for (std::size_t i = 0; i < sizeBorders; i++) {
		const Border&		border			= renderableBorders.borders[i];
		const Transform&	transform		= renderableBorders.transforms[i];
		const Texture2D		texture			= Texture2D(); // No texture
		const glm::vec4&	color			= border.getColor();
		const std::size_t&	borderThickness	= border.getBorderThickness();
		bool				inner			= border.isInner();
		const glm::ivec2&	size			= border.getSize();
		bool				clipEnabled		= border.isClipEnabled();
		unsigned short		layerIndex		= border.getLayerIndex();
		const std::vector<glm::vec2>& clipMaskVertices = border.getClipMaskVertices();


		for (std::size_t side = 0; side < 4; side++) { // 4 lines
			if (border.sideEnabled(side)) { // Check if this side is enabled (0 = top, 1 = right, 2 = bottom, 3 = left)

				glm::ivec2 rectSize = size;
				if (!inner) {
					rectSize.x += borderThickness * 2;
					rectSize.y += borderThickness * 2;
				}

				float localPosX = rectSize.x * transform.getAnchor().x; // Init with offset
				float localPosY = rectSize.y * transform.getAnchor().y; // Initi with offset


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

				glm::ivec2 borderSize;
				if (side == 0 || side == 2) {	// Vertical line
					borderSize.x = rectSize.x;
					borderSize.y = borderThickness;
				} else {						// Horizontal line
					borderSize.x = borderThickness;
					borderSize.y = rectSize.y - borderThickness * 2;
				}

				Transform lineTransform = Transform(localPosX, localPosY, transform.getZ(), TransformAnchor::TOP_LEFT, 0.0f, 1.0f);
				lineTransform.updateLocalToWorldMatrix(transform.getLocalToWorldMatrix() * lineTransform.getLocalModelMatrix());

				renderer->submit({ texture, lineTransform, borderSize, spriteShader.ID, color, clipEnabled, clipMaskVertices }, layerIndex);
			}
		}
		// Reset clipping
		renderableTexts.texts[i].resetClipping();
	}

	// Clear, Draw & Update window
	window.clear();
	renderer->flush();
	window.update();
}

void Graphics::update(float dt) {
	// Update all panels
	for (std::size_t i = 0; i < panelGroup.panels.size(); i++) {
		//std::cout << "panel update" << i << "\n";
		Panel&		panel		= panelGroup.panels[i];
		Transform&	transform	= panelGroup.transforms[i];

		const glm::vec2& size = panel.getSize();
		float x = size.x * transform.getAnchor().x;
		float y = size.y * transform.getAnchor().y;
		const glm::mat4& localToWorldMatrix = transform.getLocalToWorldMatrix();

		std::vector<GraphicComponent*> children = panel.getOwner().getComponentsInChildren<GraphicComponent>();

		glm::vec2 clipMaskVertices[4] = {
			glm::vec2(x,y),
			glm::vec2(x+size.x,y),
			glm::vec2(x+size.x,y+size.y),
			glm::vec2(x,y+size.y)
		};

		for (std::size_t i = 0; i < 4; i++) {
			clipMaskVertices[i] = localToWorldMatrix * clipMaskVertices[i];
		}

		for (GraphicComponent* child : children) {
			child->clip(clipMaskVertices);
		}//*/
	}
}

unsigned short Graphics::createLayer() {
	return renderer->createLayer();
}

Window& Graphics::getWindow() {
	return window;
}

Renderer2D& Graphics::getRenderer() {
	return *renderer;
}