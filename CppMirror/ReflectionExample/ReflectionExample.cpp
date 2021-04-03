#include "Header.h"
#include "Header2.h"
#include <iostream>
#include <fstream>
#include <functional>
#include "ReflectionPolymorph.generated.h"


void printVariable(const Mirror::VariableType& type, std::string variableName) {
	if (type.isConst)
		std::cout << "const ";
	std::cout << type.name;
	if (type.isPointer)
		std::cout << "*";
	else if (type.isReference)
		std::cout << "&";
	std::cout << " " << variableName;
}

void printFunction(const Mirror::Function& fun) {
	printVariable(fun.returnType, fun.name);
	std::cout << "(";
	std::vector<Mirror::Variable> params = fun.parameters;
	for (std::size_t i = 0; i < params.size(); i++) {
		printVariable(params[i].type, params[i].name);
		if (i != params.size() - 1)
			std::cout << ", ";
	}
	std::cout << ")\n";
}

void printProperty(const Mirror::Property& prop) {
	printVariable(prop.type, prop.name);
	std::cout << "\n";
}

void printAttributes(std::vector<Mirror::Annotation> annotations) {
	std::cout << "Attributes: {";
	for (std::size_t i = 0; i < annotations.size(); i++) {
		std::cout << annotations[i].name;
		if (i != annotations.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}\n";
	}
}

void printClass(Mirror::Class& type) {
	printAttributes(type.annotatedAttributes);
	std::cout << "Class name: " << type.name << "\n";
	for (Mirror::Property& prop : type.properties) {
		std::cout << "\t";
		printAttributes(prop.annotatedAttributes);
		std::cout << "\t\tProperty: ";
		printProperty(prop);
	}
	for (Mirror::Function& fun : type.functions) {
		std::cout << "\t";
		printAttributes(fun.annotatedAttributes);
		std::cout << "\t\tFunction: ";
		printFunction(fun);
	}
}

int main(int argc, char** argv) {
    std::cout << "Hello World!\n"; 
	// Run tests

	Foo2 foo2;
	Mirror::Class type2 = foo2.getClassType();
	printClass(type2);

	Test::Sup sup;
	Mirror::Class type3 = sup.getClassType();
	printClass(type3);

	// Invoke
	std::string string("Sup");
	std::string& stringReference = string;

	Foo2 fool;
	std::cout << "Retrieving non-existent function and calling it...\n";
	try {
		Mirror::polyInvoke(fool.getClassType().getFunction("invalidFunctionThatShouldNotExist"), &fool, 50001); // Nothing should happen (calling invalid function)
	}
	catch (const std::exception& e) {
		/**/
	}
	Mirror::polyInvoke(fool.getClassType().getFunction("isEven"), &fool, fool.getTypeID(), &stringReference);
	//

	Mirror::polySetValue(type3.getProperty("string"), &sup, sup.getTypeID(), "Hey");
	std::cout << type3.name << " the name!\n";
	std::cout << "Pre save: " << Mirror::polyGetValue<std::string>(type3.getProperty("string"), &sup, sup.getTypeID()) << "\n";

	std::ofstream saveFile;
	saveFile.open("ExampleSaveFile.txt", std::ios::out | std::ios::trunc | std::ios::binary);
	sup.serialize(saveFile);
	saveFile.close();
	std::cout << "Saved\n";
	Mirror::polySetValue(type3.getProperty("string"), &sup, sup.getTypeID(), "1337");
	std::cout << "Pre Load: " << Mirror::polyGetValue<std::string>(type3.getProperty("string"), &sup, sup.getTypeID()) << "\n";

	std::ifstream loadedFile("ExampleSaveFile.txt", std::ios::out | std::ios::binary);
	sup.deserialize(loadedFile);
	loadedFile.close();
	std::cout << "Loaded!\n";

	std::cout << "After Load: " << Mirror::polyGetValue<std::string>(type3.getProperty("string"), &sup, sup.getTypeID()) << "\n";

	if (foo2.getClassType() == foo2.getClassType())
		std::cout << "foo2.getType() == foo2.getType() is true\n";
	if (foo2.getClassType() != foo2.getClassType())
		std::cout << "foo2.getType() == foo2.getType() is false\n";

	Foo2 arrayTestClass;
	float arrayTestValue[5] = { 5,3,1,2,4.3f };
	Mirror::polyInvoke(arrayTestClass.getClassType().getFunction("arrayFunctionTest"), &arrayTestClass, arrayTestClass.getTypeID(), arrayTestValue);

	long arraySetValue[5] = { 10, 15, 13, 5, 7 };
	Mirror::polySetArrayValue(type3.getProperty("arrayTest"), &sup, sup.getTypeID(), arraySetValue);
	auto arrayTest = Mirror::polyGetArrayValue<long>(type3.getProperty("arrayTest"), &sup, sup.getTypeID());
	std::cout << "arrayTest: {";
	for (long& e : arrayTest) {
		std::cout << std::to_string(e) << ", ";
	}
	std::cout << "}\n";

	Test::Sup okay;
	short shortArraySetValue[5] = { 1, 5, 9, 85, 3 };
	Mirror::polySetArrayValue(okay.getClassType().getProperty("arrayTest"), &okay, okay.getTypeID(), shortArraySetValue);
	std::cout << "Asdf::longVar = " << Mirror::polyGetValue<int>(okay.getClassType().getProperty("longVar"), &okay, okay.getTypeID()) << "\n";
	Mirror::polySetValue(okay.getClassType().getProperty("longVar"), &okay, okay.getTypeID(), 2);
	std::cout << "Asdf::longVar set to 2\n";
	std::cout << "Asdf::longVar = " << Mirror::polyGetValue<int>(okay.getClassType().getProperty("longVar"), &okay, okay.getTypeID()) << "\n";
	auto okayV = Mirror::polyGetArrayValue<int>(okay.getClassType().getProperty("arrayTest"), &okay, okay.getTypeID());
	std::cout << "derived arrayTest: {";
	for (int& e : okayV) {
		std::cout << std::to_string(e) << ", ";
	}
	std::cout << "}\n";
	
	int testPointerTarget = 1235;
	int* testPointerArray[5] = { &testPointerTarget,&testPointerTarget,&testPointerTarget,&testPointerTarget,&testPointerTarget };
	Mirror::polySetArrayValue(okay.getClassType().getProperty("arrayPointerTest"), &okay, okay.getTypeID(), testPointerArray);
	auto okayV2 = Mirror::polyGetArrayValue<int*>(okay.getClassType().getProperty("arrayPointerTest"), &okay, okay.getTypeID());
	std::cout << "derived arrayPointerTest: {";
	for (int* e : okayV2) {
		std::cout << std::to_string(*e) << ", ";
	}
	std::cout << "}\n";

	okay.getClassType().getProperty("intVector").setArrayElementValue(&okay, 0, 1337);
	std::cout << okay.getClassType().getProperty("intVector").getArrayElementValue<int>(&okay, 0) << std::endl;
	//okay.getType().getProperty("stringVector").setArrayElementValue(&okay, 0, "hello");
	//std::cout << okay.getType().getProperty("stringVector").getArrayElementValue<std::string>(&okay, 0) << std::endl;
	Mirror::polySetArrayElementValue(okay.getClassType().getProperty("stringVector"), 0, &okay, okay.getTypeID(), "hello");
	std::cout << Mirror::polyGetArrayElementValue<std::string>(okay.getClassType().getProperty("stringVector"), 0, &okay, okay.getTypeID()) << std::endl;

	std::cout << Mirror::isReflected("Test::Sup") << std::endl;

	std::cout << okay.getClassType().getProperty("stringVector").getPointer(&okay) << std::endl;
	std::cout << "stringVector arraySize: " << okay.getClassType().getProperty("stringVector").getArraySize(&okay) << std::endl;
}