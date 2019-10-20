#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Sprite.h"
#include "components/RectTransform.h"
#include "graphics/font/Font.h"
#include "graphics/data/Color.h"
#include "graphics/data/Texture2D.h"
#include "maths/Vector2.h"
#include "Text.generated.h"

namespace Core {
	CLASS() Text : public Sprite {
		GENERATED_BODY()
	public:

		Text(std::wstring text, const char* fontAddress, int fontSize, Color color, std::size_t sortingLayer = 0);
		Text(std::string text, const char* fontAddress, int fontSize, Color color, std::size_t sortingLayer = 0);
		Text();
		~Text();

		const wchar_t* getText() const;
		const Font& getFont() const;

		void setText(std::wstring text);
		void setText(std::string text);
		Vector2 getSize();

		/* Removes current text sprites and creates new and updated sprites. */
		void refresh();
		void updateTransforms();

		const std::vector<Texture2D>& getTextSprites() const;
		const std::vector<RectTransform>& getTextTransforms() const;

	private:
		void createText();

	private:
		PROPERTY(Update=refresh())
		std::wstring text;
		PROPERTY()
		Font font;

		std::vector<Texture2D> textSprites;
		std::vector<RectTransform> textTransforms;
	};
}
#endif