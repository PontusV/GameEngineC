#include "Graphics.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Renderable2D.h"

#include <algorithm>
#include <array>
using namespace GameEngine;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Graphics::Graphics() {}


Graphics::~Graphics() {
	if (renderer)
		delete renderer;
}

void Graphics::initiate(int screenWidth, int screenHeight) {
	// Load shaders
	Shader spriteShader = ResourceManager::getInstance().loadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs");
	Shader shapeShader = ResourceManager::getInstance().loadShader("resources/shaders/shape.vs", "resources/shaders/shape.fs");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), static_cast<GLfloat>(screenHeight), 0.0f, -1.0f, 1.0f);
	// Sprite shader
	//spriteShader.use().setInteger("image", 0);
	GLint texIDs[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	};
	spriteShader.use().setIntegerV("textures", 32, texIDs);
	spriteShader.setMatrix4("projection", projection);
	// Shape shader
	shapeShader.use().setMatrix4("projection", projection);
	

	// Initialize renderer
	renderer = new Renderer2D();
}

void Graphics::render() {
	Shader defaultSpriteShader = ResourceManager::getInstance().loadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs");
	std::size_t sizeImages = renderableImages.images.size();
	std::size_t sizeTexts = renderableTexts.texts.size();

	//Add Renderables to vector IF they are inside the window. No need to sort Renderables if they cant be drawn in camera view
	//Images
	for (std::size_t i = 0; i < sizeImages; i++) {
		renderableImages.images[i].reload(); //Make sure image is loaded

		Texture2D&	texture		= renderableImages.images[i].getTexture();
		Transform&	transform	= renderableImages.transforms[i];
		glm::vec4	color		= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		renderer->submit({ texture, transform, texture.size, defaultSpriteShader.ID, color }, 0);
	}
	//Texts
	for (std::size_t i = 0; i < sizeTexts; i++) {
		renderer->renderText(renderableTexts.texts[i].getText(), renderableTexts.transforms[i], renderableTexts.texts[i].getFont(), renderableTexts.texts[i].getColor(), 0);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer->flush();
}

/* Loops through Renderable vector and draws them.*/
void Graphics::update(float dt) {
	/*if (!window.isActive()) return;
	std::size_t sizeImages = renderableImages.images.size();
	std::size_t sizeTexts = renderableTexts.texts.size();

	std::vector<Renderable> renderableVec;
	renderableVec.reserve(sizeImages + sizeTexts);
	//Add Renderables to vector IF they are inside the window. No need to sort Renderables if they cant be drawn in camera view
	//Images
	for (std::size_t i = 0; i < sizeImages; i++) {
		renderableImages.images[i].reload(window.getRenderer()); //Make sure image is loaded
		renderableVec.emplace_back(renderableImages.transforms[i], renderableImages.images[i].getTexture());
	}
	//Texts
	for (std::size_t i = 0; i < sizeTexts; i++) {
		renderableTexts.texts[i].reload(window.getRenderer()); //Make sure text is loaded
		renderableVec.emplace_back(renderableTexts.transforms[i], renderableTexts.texts[i].getTexture());
	}
	//Sort
	std::sort(renderableVec.begin(), renderableVec.end(), [](Renderable l, Renderable r) {
		return l.transform.getZ() < r.transform.getZ();
	});
	//Draw
	for (Renderable& renderable : renderableVec) {
		//Get draw variables
		int x = int(renderable.transform.getX());
		int y = int(renderable.transform.getY());
		int width = renderable.texture->getWidth();
		int height = renderable.texture->getHeight();

		//Draw
		SDL_Rect dest{ x, y, width, height };
		SDL_RenderCopy(window.getRenderer(), renderable.texture->getTexture(), NULL, &dest);
	}
	window.update();*/
}

/* Destroys any existing window and renderer and creates a new default one. */
void Graphics::createWindow() {
	//createWindow("Hello", 0, 0, 1680, 1050, 0);
}

/* Destroys any existing window and renderer and creates a new one with given parameter. */
void Graphics::createWindow(const char *title, int x, int y, int width, int height) {
	//window.create(title, x, y, width, height, flags);
}