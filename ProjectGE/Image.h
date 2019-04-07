#ifndef IMAGE_H
#define IMAGE_H

#include "GraphicComponent.h"
#include "Texture2D.h"
#include <glm/glm.hpp>
#include <string>

namespace Core {
	class Texture;

	class Image : public GraphicComponent {
		REGISTER_COMPONENT_TYPE_WITH_BASE(2, GraphicComponent);
	public:

		Image(const char* fileName, unsigned short layerIndex = 0, unsigned int width = 0, unsigned int height = 0);
		Image();

		~Image();
		// Loads image if it is not loaded
		void reload();
		Texture2D&			getTexture();
		void				setTexture(const char* fileName, Texture2D& texture);
		const char*			getFileName() const;

		// Serializable
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

	private:
		std::string 	fileName;
		Texture2D		texture;
	};
}
#endif