#pragma once
#include "Header2.h"
#include <string>
#include <vector>
//#include <ReflectionParser/ReflectionMacros.h>
#include "../ReflectionParser/include/ReflectionParser/ReflectionMacros.h"

enum OuterTestEnum {
	first = 0,
	second,
	third,
	fourth,
	fifth
};
namespace Test {
	enum class OuterClassTestEnum {
		first = 0,
		second,
		third,
		fourth
	};
#include "Sup.generated.h"
	CLASS() Sup {
	public:
		enum InnerTestEnum {
			first = 0,
			second,
			third
		};
		GENERATED_BODY()
	public:
		PROPERTY(Dumb)
		std::string string = "REEEEE";

		PROPERTY()
		std::vector<std::string> stringVector = { "test" };

		PROPERTY()
		std::vector<int> intVector = { 5, 10 };

		PROPERTY()
		std::vector<std::vector<const char*>> templateParseTester;

		PROPERTY()
		int arrayTest[5] = {5, 4, 3, 2, 1};

		PROPERTY()
		int* arrayPointerTest[5] = { &arrayTest[0], &arrayTest[1], &arrayTest[2], &arrayTest[3], &arrayTest[4] };

		PROPERTY()
		long longVar = 10L;

		PROPERTY()
		InnerTestEnum innerEnumProperty;

		PROPERTY(Category = Ayy, Update = (reload()))
		std::size_t reloadTest;
	private:
		void reload() { std::cout << "reload called!" << std::endl; }
	};
}