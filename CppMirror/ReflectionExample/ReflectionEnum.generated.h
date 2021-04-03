#ifndef REFLECTION_PARSER
#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <stdexcept>
#include "G:/source/repos/CppMirror/ReflectionExample/Header.h"

namespace Mirror{
	std::vector<std::string> getEnumConstantNames(std::string enumFullName) {
		if (enumFullName == "Test::Sup::InnerTestEnum") {
			return {"first", "second", "third"};
		}
		throw std::invalid_argument("getEnumConstantNames::ERROR The given Enum name either does not belong to any existing Enum or the Enum is not reflected.");
	}
	template <typename T>
	void setEnumConstant(T& value, std::size_t index) {
		throw std::invalid_argument("setEnumConstant::ERROR this enum type is not reflected.");
	}
	template <>
	void setEnumConstant<Test::Sup::InnerTestEnum>(Test::Sup::InnerTestEnum& value, std::size_t index) {
		if (index == 0) {
			value = Test::Sup::InnerTestEnum::first;
			return;
		}
		if (index == 1) {
			value = Test::Sup::InnerTestEnum::second;
			return;
		}
		if (index == 2) {
			value = Test::Sup::InnerTestEnum::third;
			return;
		}
		throw std::out_of_range("setEnumConstant::ERROR the given index is out of range!");
	}
}
#endif