#ifndef FONT_H
#define FONT_H
#include <string>

namespace Core {
	/* Stores a font size and an address to a ttf font. */
	class Font {
	public:
		static const char* defaultAddress;
		static int defaultSize;
		Font(const char* fileName, int size);
		Font();
		~Font();
		const char* getFileName() const;
		const int& getSize() const;

		//Save & Load operator
		template<typename Archive>
		void serialize(Archive& ar) const {
			ar(fileName, size);
		}
		template<typename Archive>
		void deserialize(Archive& ar) {
			ar(fileName, size);
		}

		std::string fileName;
		int size;
	};
}
#endif