#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Font.h"
#include "Sprite.h"
#include "Color.h"
#include "Maths/Vector2.h"
#include "Text.generated.h"

namespace Core {
	CLASS() Text : public Sprite {
		GENERATED_BODY()
	public:

		Text(std::wstring text, const char* fontAddress, int fontSize, Color color);
		Text(std::string text, const char* fontAddress, int fontSize, Color color);
		Text();
		~Text();

		const wchar_t* getText() const;
		const Font& getFont() const;

		void setText(std::wstring text);
		void setText(std::string text);
		Vector2 getSize();

	private:
		PROPERTY()
		std::wstring text;
		PROPERTY()
		Font font;
	};
}
#endif