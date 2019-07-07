#ifndef IMAGE_H
#define IMAGE_H

#include "Sprite.h"
#include "Texture2D.h"
#include "Color.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <string>
#include "Image.Image.generated.h"

namespace Core {
	class Texture;

	CLASS() Image : public Sprite {
		GENERATED_BODY()
	public:
		Image(const char* fileName, Color color = {255,255,255,255});
		Image(const char* fileName, Shader shader, Color color = {255,255,255,255});
		Image();
		~Image();

		// Loads image if it is not loaded
		void reload(bool force = false);
		Texture2D& getTexture();
		void setTexture(const char* fileName, Texture2D& texture);
		const char* getFileName() const;

		const Shader& getShader() const;
		void setShader(Shader shader);

	private:
		PROPERTY(Category=ImagePath, Update=reload(true))
		std::string fileName;
		PROPERTY()
		Texture2D texture;
		PROPERTY()
		Shader shader;
	};
}
#endif