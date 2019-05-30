#ifndef IMAGE_H
#define IMAGE_H

#include "Sprite.h"
#include "Texture2D.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <string>

namespace Core {
	class Texture;

	class Image : public Sprite {
	public:

		Image(const char* fileName, unsigned char layerIndex = 0);
		Image();

		~Image();
		// Loads image if it is not loaded
		void reload();
		Texture2D& getTexture();
		void setTexture(const char* fileName, Texture2D& texture);
		const char* getFileName() const;

		// Serializable
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

	private:
		std::string fileName;
		Texture2D texture;
	};
}
#endif