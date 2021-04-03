#ifndef FONT_H
#define FONT_H
#include "utils/Serializable.h"
#include <string>

namespace Core {
	/* Stores a font size and an address to a ttf font. */
	class Font : public Serializable {
	public:
		static const char* defaultAddress;
		static int defaultSize;
		Font(const char* fileName, int size);
		Font();
		~Font();
		const char* getFileName() const;
		const int& getSize() const;

		//Save & Load operator
		void serialize(std::ostream &os) const;
		void deserialize(std::istream &is);

		std::string fileName;
		int size;
	};
}
#endif