#ifndef IMAGE_H
#define IMAGE_H

#include "Sprite.h"
#include "Texture2D.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <string>
#include "Image.Image.generated.h"

namespace Core {
	class Texture;

	CLASS() Image : public Sprite {
		GENERATED_BODY()
	public:
		Image(const char* fileName, unsigned char layerIndex = 0);
		Image(const char* fileName, unsigned char layerIndex, Shader shader);
		Image();
		~Image();

		// Loads image if it is not loaded
		void reload(bool force = false);
		Texture2D& getTexture();
		void setTexture(const char* fileName, Texture2D& texture);
		const char* getFileName() const;

	private:
		PROPERTY(Category=ImagePath, Update=reload(true))
		std::string fileName;
		PROPERTY()
		Texture2D texture;
	};
}
#endif