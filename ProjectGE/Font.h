#ifndef FONT_H
#define FONT_H
#include <string>
#include "Serializable.h"

namespace Core {
	/* Stores an address to a ttf font and font size. Used to save and load font from file */
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
	private:
		std::string fileName;
		int size;
	};
}
#endif