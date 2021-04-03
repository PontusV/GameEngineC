#ifndef COLOR_H
#define COLOR_H
#include <iostream>
namespace Core {
	/* Holds color values between 0-255 */
	class Color {
	public:
		Color() : r(255), g(255), b(255), a(255) {}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}

		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

		unsigned char r, g, b, a;
	};
}
#endif