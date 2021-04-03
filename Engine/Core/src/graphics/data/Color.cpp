#include "Color.h"
using namespace Core;

void Color::serialize(std::ostream& os) const {
	os.write((char*)&r, sizeof(r));
	os.write((char*)&g, sizeof(g));
	os.write((char*)&b, sizeof(b));
	os.write((char*)&a, sizeof(a));
}

void Color::deserialize(std::istream& is) {
	is.read((char*)&r, sizeof(r));
	is.read((char*)&g, sizeof(g));
	is.read((char*)&b, sizeof(b));
	is.read((char*)&a, sizeof(a));
}