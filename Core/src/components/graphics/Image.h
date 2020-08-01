#ifndef IMAGE_H
#define IMAGE_H

#include "Graphic.h"
#include "graphics/data/Texture2D.h"
#include "graphics/data/Shader.h"
#include "graphics/data/Color.h"
#include <string>
#include "Image.generated.h"

namespace Core {
	CLASS() Image : public Graphic {
		GENERATED_BODY()
	public:
		Image(const char* fileName, Shader shader, Color color = { 255, 255, 255, 255 });
		Image(const char* fileName, Color color = { 255, 255, 255, 255 });
		Image();
		~Image();

		// Loads image if it is not loaded
		void reload(bool force = false);
		void set(const char* fileName);
		const char* getFileName() const;

		const Texture2D& getTexture() const;

		bool isDirty() const;
		void setDirty(bool value = true);

	private:
		PROPERTY(Category=ImagePath, Update=reload(true))
		std::string fileName;

		Texture2D texture;

		bool dirty = false;
	};
}
#endif