#pragma once
#include "dllexport.h"

struct GLFWwindow;
typedef void* (*GLADloadproc)(const char* name);
namespace Core {
	extern "C" class DLLEXPORT IWindow {
	public:
		virtual bool init() = 0;
		virtual void clear() const = 0;
		/* Swaps buffer */
		virtual void update() const = 0;
		virtual void close() = 0;

		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
		virtual bool isActive() const = 0;

		virtual void setBackgroundColor(float r, float g, float b) = 0;
		/* Expects out to be a pointer to an array of 16 floats */
		virtual void getProjectionMatrixData(float out[16]) = 0;

		virtual GLFWwindow* getWindow() = 0;
		virtual GLADloadproc getGLADloadproc() = 0;
		virtual bool initGLAD(GLADloadproc glfwGetProcAddress) = 0;
		virtual void setActive(bool value) = 0;
		virtual bool getActive() = 0;
		virtual void setResolution(float width, float height) = 0;
	};
}