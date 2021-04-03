#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "Shader.h"
#include <vector>

namespace Editor {
	class ResourceManager {
	public:
		~ResourceManager();
		static ResourceManager& getInstance() {
			static ResourceManager instance;
			return instance;
		}
		/* @param shaderFileName the name of the shader file without any file extension. */
		Shader loadShader(const std::string shaderFileName);

		// De-allocates all loaded resources
		void clear();
		void updateShader(Shader& shader);
		void updateShader(Shader& shader, const Matrix4& projection);
		// Updates Projection matrix of all shaders
		void updateShaders(const Matrix4& projection);
		// Updates View matrix of all loaded shaders
		void updateShaderViewMatrix(const Matrix4& viewMatrix);
		// Initializes all shaders. Prepares them for use in rendering.
		void initShader(const Matrix4& projection);
	private:
		ResourceManager();

		// Static  Loader functions
		static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile);

		// Resource storage
		std::map<std::string, Shader> shaders;
		Matrix4 projection;
	};
}
#endif