#include "RectMask.h"

using namespace Core;


RectMask::RectMask() {
}

RectMask::~RectMask() {
}


void RectMask::setPadding(std::size_t value) {
	padding = value;
}

std::size_t RectMask::getPadding() {
	return padding;
}