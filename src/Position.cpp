#include "Position.h"
using namespace GameEngine;



Position::Position(float x, float y) : x(x), y(y) {
} //Constructor
Position::~Position() { //Destructor
}

Position::Position() : x(0), y(0) {
}

float Position::getX() const {
	return x;
}

float Position::getY() const {
	return y;
}

void Position::setX(float value) {
	x = value;
}

void Position::setY(float value) {
	y = value;
}

void Position::serialize(std::ostream& os) const {
	os.write((char*)&x, sizeof(x));
	os.write((char*)&y, sizeof(y));
}

void Position::deserialize(std::istream& is) {
	is.read((char*)&x, sizeof(x));
	is.read((char*)&y, sizeof(y));
}

/* Load class from input stream (ex: hard drive). */
std::istream& GameEngine::operator>>(std::istream& is, Position& object) {
	object.deserialize(is);
	return is;
}

/* Save class to output stream (ex: hard drive). */
std::ostream& GameEngine::operator<<(std::ostream& os, const Position& object) {
	object.serialize(os);
	return os;
}