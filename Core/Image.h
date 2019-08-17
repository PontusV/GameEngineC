#ifndef IMAGE_H
#define IMAGE_H

#include "TexturedSprite.h"
#include "Texture2D.h"
#include "Color.h"
#include "Shader.h"
#include <string>
#include "Image.generated.h"

namespace Core {
	CLASS() Image : public TexturedSprite {
		GENERATED_BODY()
	public:
		Image(const char* fileName, Color color = {255,255,255,255});
		Image(const char* fileName, Shader shader, Color color = {255,255,255,255});
		Image();
		~Image();

		// Loads image if it is not loaded
		void reload(bool force = false);
		void set(const char* fileName);
		const char* getFileName() const;

	private:
		PROPERTY(Category=ImagePath, Update=reload(true))
		std::string fileName;
	};
}
#endif