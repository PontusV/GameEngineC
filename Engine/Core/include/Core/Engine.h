#pragma once
#include "dllexport.h"

#include "Input.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "EntityManager.h"

namespace Core {
	class DLLEXPORT IEngine {
	public:
		virtual int initiate(bool skipGLFW = false) = 0;
		/* Starts the game loop */
		virtual int start() = 0;
		/* Runs one tick of the game. Handles input, updates systems and renders */
		virtual void tick(float deltaTime) = 0;
		/* Stops the game loop and terminates glfw */
		virtual void terminate() = 0;
		/* Deletes this instance */
		virtual void release() = 0;

		virtual void resizeViewport(unsigned int width, unsigned int height) = 0;

		// Systems
		virtual IInput* getInputInterface() = 0;
		virtual IGraphics* getGraphicsInterface() = 0;
		virtual ISceneManager* getSceneManagerInterface() = 0;
		virtual IEntityManager* getEntityManagerInterface() = 0;
	};
}