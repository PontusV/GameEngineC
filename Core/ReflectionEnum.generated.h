#ifndef REFLECTION_PARSER
#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <stdexcept>
#include "G:/Projects/ProjectGE/Core/Alignment.h"

namespace Mirror{
	std::vector<std::string> getEnumConstantNames(std::string enumFullName) {
		if (enumFullName == "Core::Alignment") {
			return {"TOP_LEFT", "TOP", "TOP_RIGHT", "LEFT", "CENTER", "RIGHT", "BOTTOM_LEFT", "BOTTOM", "BOTTOM_RIGHT"};
		}
		throw std::invalid_argument("getEnumConstantNames::ERROR The given Enum name either does not belong to any existing Enum or the Enum is not reflected.");
	}
	template <typename T>
	void setEnumConstant(T& value, std::size_t index) {
		throw std::invalid_argument("setEnumConstant::ERROR this enum type is not reflected.");
	}
	template <>
	void setEnumConstant<Core::Alignment>(Core::Alignment& value, std::size_t index) {
		if (index == 0) {
			value = Core::Alignment::TOP_LEFT;
			return;
		}
		if (index == 1) {
			value = Core::Alignment::TOP;
			return;
		}
		if (index == 2) {
			value = Core::Alignment::TOP_RIGHT;
			return;
		}
		if (index == 3) {
			value = Core::Alignment::LEFT;
			return;
		}
		if (index == 4) {
			value = Core::Alignment::CENTER;
			return;
		}
		if (index == 5) {
			value = Core::Alignment::RIGHT;
			return;
		}
		if (index == 6) {
			value = Core::Alignment::BOTTOM_LEFT;
			return;
		}
		if (index == 7) {
			value = Core::Alignment::BOTTOM;
			return;
		}
		if (index == 8) {
			value = Core::Alignment::BOTTOM_RIGHT;
			return;
		}
		throw std::out_of_range("setEnumConstant::ERROR the given index is out of range!");
	}
}
#endif