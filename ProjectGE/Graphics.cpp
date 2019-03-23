#include "Graphics.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Renderable2D.h"

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
	const int amountofLayers = 2;				//Game layer, UI layer
	renderer = new Renderer2D(&window, amountofLayers);

	// Succesfull initialization
	return true;
}

/* Loops through Renderable vector and draws them.*/
void Graphics::render() {
	std::size_t sizeRectangles	= renderableRects.rects.size();
	std::size_t sizeImages		= renderableImages.images.size();
	std::size_t sizeTexts		= renderableTexts.texts.size();

	// TODO: Add Renderables to vector IF they are inside the window. No need to sort Renderables if they cant be drawn in camera view
	// Rectangles
	for (std::size_t i = 0; i < sizeRectangles; i++) {
		const Texture2D		texture			= Texture2D(); // No texture
		const Transform&	transform		= renderableRects.transforms[i];
		const glm::vec4&	color			= renderableRects.rects[i].getColor();
		const glm::ivec2&	size			= renderableRects.rects[i].getSize();
		const glm::vec2&	figureOffset	= renderableRects.rects[i].getOffset();
		bool				clipEnabled		= renderableRects.rects[i].isClipEnabled();
		const glm::vec4&	drawRect		= renderableRects.rects[i].getDrawRect();
		unsigned short		layerIndex		= renderableRects.rects[i].getLayerIndex();

		renderer->submit({ texture, transform, size, spriteShader.ID, color, figureOffset, clipEnabled, drawRect }, layerIndex);
	}
	// Images
	for (std::size_t i = 0; i < sizeImages; i++) {
		renderableImages.images[i].reload(); //Make sure image is loaded

		const Texture2D&	texture		= renderableImages.images[i].getTexture();
		const Transform&	transform	= renderableImages.transforms[i];
		const glm::vec4		color		= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		const glm::vec2&	imageOffset	= renderableImages.images[i].getOffset();
		bool				clipEnabled = renderableImages.images[i].isClipEnabled();
		const glm::vec4&	drawRect	= renderableImages.images[i].getDrawRect();
		unsigned short		layerIndex	= renderableImages.images[i].getLayerIndex();

		renderer->submit({ texture, transform, texture.size, spriteShader.ID, color, imageOffset, clipEnabled, drawRect }, layerIndex);
	}

	// Texts
	for (std::size_t i = 0; i < sizeTexts; i++) {
		bool				clipEnabled = renderableTexts.texts[i].isClipEnabled();
		const glm::vec4&	drawRect	= renderableTexts.texts[i].getDrawRect();
		unsigned short		layerIndex	= renderableTexts.texts[i].getLayerIndex();

		renderer->renderText(renderableTexts.texts[i].getText(), renderableTexts.transforms[i], renderableTexts.texts[i].getFont(), renderableTexts.texts[i].getColor(), clipEnabled, drawRect, layerIndex);
	}

	// Clear, Draw & Update window
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer->flush();
	window.update();
}


void Graphics::update(float dt) {
	// Update all panels
	for (std::size_t i = 0; i < panelGroup.panels.size(); i++) {
		Panel&		panel		= panelGroup.panels[i];
		Transform&	transform	= panelGroup.transforms[i];

		float x = transform.getX() + panel.getOffset().x;
		float y = transform.getY() + panel.getOffset().y;
		int w = panel.getSize().x;
		int h = panel.getSize().y;

		std::vector<GraphicComponent*> children = panel.getGraphicChildren();

		for (GraphicComponent* child : children) {
			child->enableClipping();
			child->setDrawRect(glm::vec4(x, y, w, h));
		}
	}
}

Window& Graphics::getWindow() {
	return window;
}