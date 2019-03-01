#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "Component.h"
#include "Texture2D.h"

namespace GameEngine {
	class Texture;

	class Image : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 2;
		Image(const std::string fileName, unsigned int width = 0, unsigned int height = 0);
		Image(std::istream& is);
		Image();

		~Image();
		//Loads image if it is not loaded
		void reload();
		Texture2D& getTexture();
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		//Save & Load operator
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);
	private:

		std::string fileName;
		Texture2D texture;
		unsigned int width, height; //Size to draw texture
	};
}
#include "TypeList.h"
typedef typename GameEngine::TypeList<TYPE_LIST, GameEngine::Image>::type TypeListImage;
#undef TYPE_LIST
#define TYPE_LIST TypeListImage
#endif