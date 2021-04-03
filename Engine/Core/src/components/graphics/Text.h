#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>
#include "Graphic.h"
#include "graphics/font/Font.h"
#include "graphics/data/Texture2D.h"
#include "components/RectTransform.h"
#include "Text.generated.h"

namespace Core {
	struct TextSprite {
		Texture2D texture;
		RectTransform transform;
	};

	CLASS() Text : public Graphic {
		GENERATED_BODY()
	public:
		Text(std::wstring text, Font font, Color color = { 255, 255, 255, 255 });
		Text(std::string text, Font font, Color color = { 255, 255, 255, 255 });
		Text();
		~Text();

		void setText(std::wstring text);
		void setText(std::string text);
		const std::wstring& getText() const;

		void setFont(Font value);
		const Font& getFont() const;

		void setDirty(bool value = true);
		bool isDirty() const;

		/* Replaces dirty sprites with new sprites */
		void update(std::vector<TextSprite> sprites);
		const std::vector<TextSprite>& getSprites() const;

	private:
		PROPERTY(Update = setDirty(true))
		std::wstring text;
		PROPERTY(Update = setDirty(true))
		Font font;

		bool dirty = false;

		std::vector<TextSprite> sprites;
	};
}
#endif