#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include "Component.h"
#include "HandleManager.h"
#include "ResourceManager.h"

namespace GameEngine {
	class Texture;

	class Image : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 2;
		Image(const std::string fileName);
		Image(std::istream& is);
		Image();

		~Image();
		void reload(SDL_Renderer* ren);
		TexturePtr getTexture();
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		//Save & Load operator
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);
	private:

		std::string fileName;
		TexturePtr texture;
	};
}
#include "TypeList.h"
typedef typename GameEngine::TypeList<TYPE_LIST, GameEngine::Image>::type TypeListImage;
#undef TYPE_LIST
#define TYPE_LIST TypeListImage
#endif