#pragma once
#include <string>
//#include <ReflectionParser/ReflectionMacros.h>
#include "../ReflectionParser/include/ReflectionParser/ReflectionMacros.h"
#include "Foo2.generated.h"

CLASS() Foo2 {
	GENERATED_BODY()
public:
	Foo2() {}

	FUNCTION(world)
	static bool arrayFunctionTest(float floatArray[5]) {
		std::cout << "Foo2::arrayFunctionTest called! {";
		for (std::size_t i=0;i<5;i++) {
			std::cout << std::to_string(floatArray[i]);
			if (i != 4)
				std::cout << ", ";
		}
		std::cout << "}\n";
		return true;
	}
private:
	FUNCTION()
	void isEven(const std::string* constStringReference) const {
		std::cout << "Foo2::isEven called with " << *constStringReference << "\n";
	}

	FUNCTION(world)
	static bool staticMethod(float cloud, int integer, long notInteger) {
		std::cout << "Foo2::staticMethod called!\n";
		return true;
	}

	FUNCTION(world)
		static bool arrayFunctionTest2(float floatArray[10]) {
		std::cout << "Foo2::arrayFunctionTest called! {";
		for (std::size_t i = 0; i < 10; i++) {
			std::cout << std::to_string(floatArray[i]);
			if (i != 9)
				std::cout << ", ";
		}
		std::cout << "}\n";
		return true;
	}

	int integerPointerTarget = 10;

	PROPERTY()
	const std::string constStringProperty;

	PROPERTY()
	int* integer = &integerPointerTarget;

	PROPERTY()
	static bool staticBoolean;

	PROPERTY()
	const float constFloat = 5;

	PROPERTY()
	float simpleFloat = 0;
};

bool Foo2::staticBoolean = false;